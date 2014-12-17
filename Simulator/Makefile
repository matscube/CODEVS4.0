JAVAC = javac -J-Dfile.encoding=UTF8

default: java_compile
	java CodeVS40Simulator -exec "./SnowCleaning"

java_only: java_compile
	java CodeVS40Simulator

java_compile:
	$(JAVAC) CodeVS40Simulator.java -Xlint:unchecked

field_sample:
	$(JAVAC) FieldSample.java -Xlint:unchecked
	java FieldSample