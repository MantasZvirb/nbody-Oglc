
void pc(char *Text);
char *LoadFile(char *Path,unsigned long *OutSize);

char ChunkToData(char *Data,
unsigned long DataSize,char *ChunkName,
unsigned long *OutDataSize,
float **OutData,unsigned short **OutIntDta);
