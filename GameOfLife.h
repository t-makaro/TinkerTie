//Game of Life
boolean game(byte ledNum){
	byte x = 8;
	byte y = 5;
	byte onoff = 1 << 7;
	static byte board[y][x] = {
		{28, 0, 0, 0, 0, 0,0,1},
		{27,20, 0, 0, 0, 0,9,2},
		{26,21,19, 0, 0,10,8,3},
		{25,22,18,15,14,11,7,4},
		{24,23,17,16,13,12,6,5}
	};
	byte tempBoard[y][x];
	//iterate board
	for(byte i; i < x; i++){
		for(byte j; j < y; j++){
			10000000
		}
	}
	
}

boolean on(byte piece){
	return (piece >> 7) & 1;
}