/*
Navicat MySQL Data Transfer

Source Server         : mysql
Source Server Version : 50630
Source Host           : localhost:3306
Source Database       : book

Target Server Type    : MYSQL
Target Server Version : 50630
File Encoding         : 65001

Date: 2017-06-14 22:32:49
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for book
-- ----------------------------
DROP TABLE IF EXISTS `book`;
CREATE TABLE `book` (
  `bookid` int(11) NOT NULL AUTO_INCREMENT,
  `del` tinyint(1) DEFAULT '0',
  `operator` varchar(30) DEFAULT NULL,
  `intime` date DEFAULT NULL,
  `bookcase` varchar(10) DEFAULT NULL,
  `page` int(10) DEFAULT NULL,
  `ISBN` varchar(20) DEFAULT NULL,
  `translator` varchar(30) DEFAULT NULL,
  `autor` varchar(30) DEFAULT NULL,
  `type` int(10) DEFAULT NULL,
  `bookname` varchar(70) DEFAULT NULL,
  `barcode` varchar(30) DEFAULT NULL,
  `price` float(8,2) DEFAULT NULL,
  PRIMARY KEY (`bookid`)
) ENGINE=InnoDB AUTO_INCREMENT=1005 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of book
-- ----------------------------
INSERT INTO `book` VALUES ('1001', '0', '小明', '2017-06-01', '左A-1', '300', '清华大学出版社', '李华', '岳磊', '1', 'JSP应用开发与实践', '9787115195975', '89.00');
INSERT INTO `book` VALUES ('1002', '0', '小明', '2017-06-01', '左A-2', '400', '北京大学出版社', '李华', '邓超', '1', 'JAVA基础', '97871151944', '80.00');
INSERT INTO `book` VALUES ('1003', '0', '小红', '2017-06-01', '左A-1', '250', '清华大学出版社', '李华', '汪涵', '2', '.NET开发', '97371151922', '70.00');
INSERT INTO `book` VALUES ('1004', '1', '小红', '2017-06-01', '右A-2', '400', '北京大学出版社', '李华', '何炅', '4', '青春无极限', '2237115192223371151922', '45.00');

-- ----------------------------
-- Table structure for borrow
-- ----------------------------
DROP TABLE IF EXISTS `borrow`;
CREATE TABLE `borrow` (
  `borrowid` int(10) NOT NULL AUTO_INCREMENT,
  `readerid` int(10) DEFAULT NULL,
  `bookid` int(10) DEFAULT NULL,
  `borrowTime` date DEFAULT NULL,
  `backtime` date DEFAULT NULL,
  `operator` varchar(30) DEFAULT NULL,
  `ifback` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`borrowid`),
  KEY `bookid` (`bookid`),
  KEY `readerid` (`readerid`),
  CONSTRAINT `bookid` FOREIGN KEY (`bookid`) REFERENCES `book` (`bookid`),
  CONSTRAINT `readerid` FOREIGN KEY (`readerid`) REFERENCES `reader` (`readerid`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of borrow
-- ----------------------------
INSERT INTO `borrow` VALUES ('1', '111', '1001', '2017-06-01', '2017-08-01', '小明', '0');
INSERT INTO `borrow` VALUES ('2', '112', '1002', '2017-06-01', '2017-08-01', '小明', '0');
INSERT INTO `borrow` VALUES ('3', '113', '1003', '2017-06-01', '2017-08-01', '小红', '0');
INSERT INTO `borrow` VALUES ('4', '113', '1001', '2017-06-01', '2017-06-13', '小红', '1');

-- ----------------------------
-- Table structure for manager
-- ----------------------------
DROP TABLE IF EXISTS `manager`;
CREATE TABLE `manager` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `name` varchar(30) DEFAULT NULL,
  `password` varchar(30) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of manager
-- ----------------------------
INSERT INTO `manager` VALUES ('1', 'yue', '123');
INSERT INTO `manager` VALUES ('2', 'wang', '123');
INSERT INTO `manager` VALUES ('3', 'Li', '123');
INSERT INTO `manager` VALUES ('4', 'yuelei', '123');

-- ----------------------------
-- Table structure for reader
-- ----------------------------
DROP TABLE IF EXISTS `reader`;
CREATE TABLE `reader` (
  `readerid` int(10) NOT NULL AUTO_INCREMENT,
  `name` varchar(20) DEFAULT NULL,
  `sex` varchar(4) DEFAULT NULL,
  `vocation` varchar(50) DEFAULT NULL,
  `birthday` date DEFAULT NULL,
  `paperType` varchar(10) DEFAULT NULL,
  `paperNumber` varchar(20) DEFAULT NULL,
  `phone` varchar(20) DEFAULT NULL,
  `email` varchar(100) DEFAULT NULL,
  `Date` date DEFAULT NULL,
  `remark` text,
  `type` int(11) DEFAULT NULL,
  PRIMARY KEY (`readerid`)
) ENGINE=InnoDB AUTO_INCREMENT=114 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of reader
-- ----------------------------
INSERT INTO `reader` VALUES ('111', '岳磊', '1', '学生', '1993-01-28', '身份证', '411234199345677654', '13612382282', '123456@qq.com', '2017-06-01', '', '1');
INSERT INTO `reader` VALUES ('112', '郭德纲', '1', '演员', '1974-04-04', '身份证', '123456789123456789', '12345678901', '3456789@qq.com', '2017-06-01', null, '2');
INSERT INTO `reader` VALUES ('113', '小月', '0', '老师', '1989-06-06', '身份证', '345678901234567890', '23456789012', '777767@qq.com', '2017-06-01', null, '3');
