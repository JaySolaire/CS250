
char* circbuffer(*data_in, char *circbuffer, size_of_circbuffer)
{
	int counter = 0;
	while (*data_in)
	{
		//*(circbuffer + counter%size_of_circbuffer) = *data_in //SAME THING JUST IN POINTER FORMAT
		circbuffer[counter%size_of_circbuffer] = *data_in;
		counter++;
	}
	return circbuffer;
}

//server, circular buffer posting data 50 chars per line

//client, reading in data 10 bytes per line
//then printing it to the screen




