JAVAC = javac -J-Dfile.encoding=UTF8

default: java_compile
	java CodeVS40Simulator -exec "./SnowCleaning"

java_only: java_compile
	java CodeVS40Simulator

java_compile:
	$(JAVAC) CodeVS40Simulator.java -Xlint:unchecked

game:
	$(JAVAC) CodeVS.java -Xlint:unchecked
	java CodeVS