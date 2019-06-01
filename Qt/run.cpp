// g++ -fPIC --std=c++11 `pkg-config --cflags --libs Qt5Core` -o run run.cpp

#include <QDebug>
#include <QString>
#include <QProcess>

int main(int argc, char *argv[]) {
	
	if (argc <= 1) {
		qDebug() << "Usage:" << QString::fromLocal8Bit(argv[0]) << "command";
	}
	
	QStringList rawCommandList;

	for (int i = 1; i < argc; i++) {
		QString currentArg = QString::fromLocal8Bit(argv[i]);
		rawCommandList.append(currentArg);
	}
	
	QProcess::startDetached(rawCommandList.join(' '));

	return 0;
}

