// function to read a a plain text file into memory
int ReadFile(FILE* fp, int length, char* shader){
	if (fp){
	       	int amountRead = fread(shader, 1, length, fp);
		if (amountRead > 1) return amountRead;
		else return -1;
	}
	else return -1;

}
// function to read length of a plain text file
int ReadLength(FILE* fp){
	if (fp){
		fseek (fp, 0, SEEK_END);
		int length = ftell (fp);
		fseek (fp, 0, SEEK_SET);
		return length;
	} else return -1;
}
