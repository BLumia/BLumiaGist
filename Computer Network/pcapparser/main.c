#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/in.h> // IPPROTO_TCP
#include <netinet/tcp.h>
#include "pcapstruct.h"

typedef unsigned char   byte;

// struct tcphdr
char* prg_name;
pcap_hdr_t pcap_global_hdr;
pcaprec_hdr_t pcap_packet_hdr;
struct ethhdr eth_hdr;
struct iphdr ip_hdr, ip_hdr_for_cksum;
struct tcphdr tcp_hdr, tcp_hdr_for_cksum;
struct tcp4_pseudohdr tcp_pseudo_hdr;
int native_byte_order;
off_t next_packet_location;
off_t end_of_file_pos;
byte cksum_buffer[888], tcpbody[888];

void hex_dump(const char *buf, int len) {
	const char* addr = buf;
    int i,j,k;
    char binstr[80];
 
    for (i=0;i<len;i++) {
        if (0==(i%16)) {
            sprintf(binstr,"%08x -",i+addr);
            sprintf(binstr,"%s %02x",binstr,(byte)buf[i]);
        } else if (15==(i%16)) {
            sprintf(binstr,"%s %02x",binstr,(byte)buf[i]);
            sprintf(binstr,"%s  ",binstr);
            for (j=i-15;j<=i;j++) {
                sprintf(binstr,"%s%c",binstr,('!'<buf[j]&&buf[j]<='~')?buf[j]:'.');
            }
            printf("%s\n",binstr);
        } else {
            sprintf(binstr,"%s %02x",binstr,(byte)buf[i]);
        }
    }
    if (0!=(i%16)) {
        k=16-(i%16);
        for (j=0;j<k;j++) {
            sprintf(binstr,"%s   ",binstr);
        }
        sprintf(binstr,"%s  ",binstr);
        k=16-k;
        for (j=i-k;j<i;j++) {
            sprintf(binstr,"%s%c",binstr,('!'<buf[j]&&buf[j]<='~')?buf[j]:'.');
        }
        printf("%s\n",binstr);
    }
}

uint16_t in_cksum(byte *ptr, int size) { 

	int cksum = 0;  
	int index = 0;   

	if(size % 2 != 0) return 0;  
		
	while(index < size) {          
		cksum += *(ptr + index + 1);  
		cksum += *(ptr + index) << 8;  
		index += 2;  
	} 

	while(cksum > 0xffff) {  
		cksum = (cksum >> 16) + (cksum & 0xffff);  
	} 
	return ~cksum;  
}  

int main(int argc, char** argv) {

	prg_name = strrchr(argv[0], '/');
	if(prg_name) prg_name++;
	else prg_name = argv[0];

	if (argc != 2) {
		printf("Usage:%s <pcap_file_path>\n", prg_name);
	}

	int filefd = open(argv[1], O_RDONLY);
	if (filefd == -1) {
		perror("open()");
		exit(0);
	}

	end_of_file_pos = lseek(filefd, 0, SEEK_END);
	lseek(filefd, 0, SEEK_SET);

	read(filefd, &pcap_global_hdr, sizeof(pcap_hdr_t));
	
	if (pcap_global_hdr.magic_number == 0xa1b2c3d4) {
		native_byte_order = 1;
	} else if (pcap_global_hdr.magic_number == 0xd4c3b2a1) {
		native_byte_order = 0;
	} else {
		puts("Not a pcap file or unsupported byte order.Exit...\n");
		exit(0);
	}

	printf("Libpcap File Loaded:\nVersion: %u.%u\tByte order: %s\n", 
				pcap_global_hdr.version_major, pcap_global_hdr.version_minor, native_byte_order == 1 ? "Host" : "Swap");

again:

	read(filefd, &pcap_packet_hdr, sizeof(pcaprec_hdr_t));
	printf("New Packet Reached:\nPacket Size: %u Byte / %u Byte (Readed / Actual Size)\n", 
				pcap_packet_hdr.incl_len, pcap_packet_hdr.orig_len);

	next_packet_location = lseek(filefd, 0, SEEK_CUR) + pcap_packet_hdr.incl_len;
	
	read(filefd, &eth_hdr, sizeof(struct ethhdr));
	int ipv4_inside = ntohs(eth_hdr.h_proto) == ETH_P_IP ? 1 : 0;
	printf("\t> Ethernet II Reached, inside data type: %04X(%s)\n", 
				ntohs(eth_hdr.h_proto), ipv4_inside ? "IPv4" : "Unknown");

	if (ipv4_inside) {
		read(filefd, &ip_hdr, sizeof(struct iphdr));
		memcpy(&ip_hdr_for_cksum, &ip_hdr, sizeof(ip_hdr));
		ip_hdr_for_cksum.check = 0x0000;
		__sum16 ip_cksum = in_cksum((byte*)&ip_hdr_for_cksum, sizeof(struct iphdr));
		int tcp_inside = ip_hdr.protocol == IPPROTO_TCP ? 1 : 0;
		printf("\t> IPv4 Packet Reached, inside data type: %04X(%s), header checksum: %04X | %04X (Caculated | iphdr.check)\n",
				ntohs(ip_hdr.protocol), tcp_inside ? "TCP" : "Unknown", ntohs(ip_cksum), ip_hdr.check);
		
		if (tcp_inside) {
			read(filefd, &tcp_hdr, sizeof(struct tcphdr));

			tcp_pseudo_hdr.saddr = ip_hdr.saddr;
			tcp_pseudo_hdr.daddr = ip_hdr.daddr;
			tcp_pseudo_hdr.pad = 0x00;
			tcp_pseudo_hdr.protocol = IPPROTO_TCP;
			tcp_pseudo_hdr.len = htons(ntohs(ip_hdr.tot_len) - (ip_hdr.ihl * 4));

			memcpy(&tcp_hdr_for_cksum, &tcp_hdr, sizeof(struct tcphdr));
			tcp_hdr_for_cksum.check = 0x0000;

			int cksum_buffer_len = ntohs(ip_hdr.tot_len) - (ip_hdr.ihl * 4) + sizeof(tcp_pseudo_hdr);
			//printf("---%d---%d---%d---\n", cksum_buffer_len, ntohs(ip_hdr.tot_len), sizeof(tcp_hdr));
			read(filefd, &tcpbody, ntohs(tcp_pseudo_hdr.len) - sizeof(tcp_hdr));
			memcpy(&cksum_buffer, &tcp_pseudo_hdr, sizeof(tcp_pseudo_hdr));
			memcpy(cksum_buffer + sizeof(tcp_pseudo_hdr), &tcp_hdr_for_cksum, sizeof(tcp_hdr_for_cksum));
			memcpy(cksum_buffer + sizeof(tcp_pseudo_hdr) + sizeof(tcp_hdr), &tcpbody, cksum_buffer_len - sizeof(tcp_hdr));
			__sum16 tcp_cksum = in_cksum((byte*)cksum_buffer, cksum_buffer_len);
			printf("\t> TCP Packet Reached, checksum: %04X | %04X (Caculated | tcphdr.check)\n",
						ntohs(tcp_cksum), tcp_hdr.check);

			//hex_dump(&cksum_buffer, cksum_buffer_len);
			//hex_dump(&tcp_cksum, sizeof(__sum16));
		}
	}

	//hex_dump(&eth_hdr, sizeof(eth_hdr));
	if (lseek(filefd, next_packet_location, SEEK_SET) < end_of_file_pos - 1) goto again;
	puts("Tada!");

}
