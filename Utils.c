#include <stdio.h>
#include <stdlib.h>

void pc(char *Text)
{
	printf("%s\n",Text);
}

char *LoadFile(char *Path,unsigned long *OutSize)
{
	FILE *Fd = fopen(Path,"rb");
	unsigned long Fs;
	fseek(Fd,0,SEEK_END);
	Fs = ftell(Fd);
	rewind(Fd);

	char *Data = malloc(Fs);
	unsigned long OutSizeFile = 0;
	OutSizeFile = fread(Data,1,Fs,Fd);
	if(OutSizeFile != Fs)
	{
		free(Data);
		close(Fd);
		return 0;
	}
	if(OutSize != 0)
	{
		*OutSize = Fs; 
	}
	close(Fd);
	return Data;
}

unsigned long StrSz(char *Str1)
{
	unsigned long i = 0;
	while(Str1[i] != 0)
	{
		i++;
	}
	return 0;
}

char StrCmp(char *Str1,char *Str2)
{
	unsigned long l1 = StrSz(Str1);
	unsigned long l2 = StrSz(Str2);
	if(l1!=l2)
	{
		return 0;
	}
	for(unsigned long i = 0;i < l1;i++)
	{
		if(Str1[i] != Str2[i])
		{
			return 0;
		}
	}
	return 1;
}

char *FindChunk(char *Data,unsigned long DataSize,
char *Name,unsigned long *OutChunkSize)
{
	unsigned long ChunkSize = 0;
	char TempBuff[100] = {0};
	unsigned long TI = 0;
	for(unsigned long i = 0;i < DataSize;i++)
	{
		if(Data[i] == '[')
		{
			i++;
			while(Data[i] != ']' && TI < 100)
			{
				TempBuff[TI] = Data[i];
				TI++;
				i++;
			}
			if(strcmp(TempBuff,Name) == 0)
			{
				i++;
				char *ChunkPtr = &Data[i];
				unsigned long Len = 0;
				while(Data[i] != ';')
				{
					Len++;
					i++;
				}
				*OutChunkSize = Len;
				return ChunkPtr;
			}
			else
			{
				memset(TempBuff,0,sizeof(TempBuff));
				TI = 0;
			}	
		}
	}
	return 0; 
}

typedef struct LinkedList
{
	struct LinkedList *Next;
	char *Data;
	unsigned long DataSize;
}LinkedList;

void LinkedListAddPtrCreate(LinkedList **Base,
LinkedList **OutCurrent,
char *DataPtr,unsigned long DataSize)
{
	LinkedList *Ls = (LinkedList*)malloc(sizeof(LinkedList));
	Ls->Data = DataPtr;
	Ls->DataSize = DataSize;
	Ls->Next = 0;
	if(*Base == 0)
	{
		*Base = Ls;
	}
	else
	{
		LinkedList *Current = *Base;
		while(1)
		{
			if(Current->Next == 0)
			{
				break;
			}
			Current = Current->Next;				
		}
		Current->Next = Ls;
		if(OutCurrent != 0)
		{
			*OutCurrent = Ls;
		}
	}
}

void LinkedListClear(LinkedList *Ls)
{
	LinkedList *Cl = Ls;
	LinkedList *Dl = Ls;	
	while(1)
	{
		if(Cl == 0)
		{
			break;
		}
		Cl = Cl->Next;
		free(Dl);
		Dl = Cl;
	}
}

char ChunkToData(char *Data,
unsigned long DataSize,char *ChunkName,
unsigned long *OutDataSize,
float **OutData,unsigned short **OutIntDta)
{
	unsigned long ChunkSize = 0;
	LinkedList *DataList = 0;
	char *ChunkPtr = FindChunk(Data,DataSize,ChunkName,&ChunkSize);
	
	if(ChunkPtr == 0)
	{
		return 0;
	}
	
	for(unsigned long i = 0; i < ChunkSize;)
	{
		while(ChunkPtr[i] == '\n' || ChunkPtr[i] == ' ' 
		|| ChunkPtr[i] == '\t' || ChunkPtr[i] == '\r')
		{
			i++;
		}
		char Flg = 0;
		char *Ptr = &ChunkPtr[i];
		
		while(ChunkPtr[i] != '\n' && ChunkPtr[i] != ' ' 
		&& ChunkPtr[i] != '\t' && 
		ChunkPtr[i] != '\r' && ChunkPtr[i] != ';')
		{
			i++;
		}
		if(ChunkPtr[i] == ';')
		{
			Flg = 1;
		}
		unsigned long DataSize = &ChunkPtr[i]-Ptr;
		LinkedListAddPtrCreate(&DataList,0,Ptr,DataSize);
		if(Flg == 1)
		{
			break;
		}
	}

	unsigned long NumOfElements = 0;
	LinkedList *Cl = DataList;	
	while(1)
	{
		Cl = Cl->Next;
		NumOfElements++;
		if(Cl == 0)
		{
			break;
		}
	}
	Cl = DataList;
	float *DataOut = 0;
	unsigned short *ShortDta = 0;
	if(OutData != 0)
	{
		DataOut = (float*)malloc(
		sizeof(float)*NumOfElements);
	}
	else
	{
		ShortDta = (unsigned short)malloc(
		sizeof(unsigned short)*NumOfElements);
		
		ShortDta = malloc(100000);
	}
	unsigned long Index = 0;
	while(1)
	{
		char Temp[100] = {0};
		for(unsigned long i = 0; i < Cl->DataSize && i < 100;i++)
		{
			Temp[i] = Cl->Data[i];
		}
		
		if(OutData != 0)
		{
			float Dta = (float)atof(Temp);
			DataOut[Index] = Dta;
		}
		else
		{
			unsigned short Dt = (unsigned short)atoi(Temp);
			ShortDta[Index] = Dt;
		}
		
		Cl = Cl->Next;
		Index++;
		if(Cl == 0)
		{
			break;
		}
	}
	LinkedListClear(DataList);
	if(OutData != 0)
	{
		*OutData = DataOut;
		*OutDataSize = NumOfElements*sizeof(float);
	}
	else
	{
		*OutIntDta = ShortDta;
		*OutDataSize = NumOfElements*sizeof(short);
	}
	return 1;
}


