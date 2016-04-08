/*
 Name:		WiFi_storage.ino
 Created:	4/8/2016 8:40:28 PM
 Author:	Pranav
*/
#include <SPI.h>
#include <SD.h>
// the setup function runs once when you press reset or power the board
#define CS_PIN 4
#define MAX_BUFFER_LEN 512

File myFile;

void setup()
{
	// Open serial communications and wait for port to open:
	Serial.begin(9600);

	Serial.print("Initializing SD card...");

	if (!SD.begin(CS_PIN)) {
		Serial.println("initialization failed!");
		return;
	}
	Serial.println("initialization done.");
	
	
}
// this function fills the buffer upto specified size and then
//returns the no. of bytes of data left of the file
//if we fixx the buffer length to 512 which is more than enough given the status of
//our ram 
 long  getFileData(char * filename,unsigned char * buffer,long readFrom ){
	 unsigned int i = 0;
	 //buffer is an array that contains the most recent data and it extends till 'size'
	if(SD.exists(filename)) {
		Serial.println("requested file exists.");
		//read the file into the buffer of length specified by size
		myFile = SD.open(filename, FILE_READ);
		if (myFile) {
			myFile.seek(readFrom);
			if (myFile.available()){
				for ( i = 0; i < MAX_BUFFER_LEN && myFile.available()>0; i++){
					unsigned char read = myFile.read();
//					Serial.write(read);
					*(buffer++) = read;
				}
			}
			else{
				myFile.close();//close file
				return -1;
			}
			myFile.close();//close file
		}
		else {
			// if the file didn't open, print an error:
			Serial.println("error opening file");
			return -1;
		}

	}
	else { Serial.println("requested file doesn't exist."); return -1; }
	
	return readFrom + i;//indicate file end too


}
 unsigned char buff[MAX_BUFFER_LEN];
void loop()
{
	// nothing happens after setup finishes.
	
	unsigned int nextChunk = 0;
	while (nextChunk != -1){
		nextChunk = getFileData("ARDUINO.ino", buff, nextChunk);
		for (int i = 0; i < (nextChunk%MAX_BUFFER_LEN == 0 ? MAX_BUFFER_LEN : nextChunk%MAX_BUFFER_LEN) && nextChunk != -1; i++){
			Serial.print((char)buff[i]);
		}
		Serial.println();
		Serial.println(nextChunk);
		delay(2000);
	}
	delay(10000);

}


