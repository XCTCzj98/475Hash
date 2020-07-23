#include<bits/stdc++.h> 
#include <stdint.h>
using namespace std;

string Keccak_f(string b)
{
	//In SHA-3 we implement the length of 1600(longest)
	//Input: a string b of length 1600
	//Output: a string of length 1600 after 24 rounds stir
}

uint64_t Keccak_frot(uint64_t num, int offset) {
    //Input: @num: It is one 64bits long unsigned int     
    //       @offset: It is the number of moving bits
	//Requirement: offset should between 0 and 64
	//             | because in some cases, the moving bits will be 0 if couted only one way 
    return (num << offset) | (num >> (64 - offset));
}

void Keccak_ftheta(uint64_t input[5][5], uint64_t output[5][5]) 
{
	//Input: @input 5*5 matrix of uint64_t, which store the matrix before theta transformation
	//       @output 5*5 matrix of uint64_t, which store the matrix after theta transformation
	// Three things we need to do:(i all from 0 to 4)
	// 1.Calculate C[i], for C[i]=A[i,0] xor A[i,1] xor A[i,2] xor A[i,3] xor A[i,4]
	// 2.Calculate D[i], for D[i]=C[i-1] xor Keccak_frot(C[i+1],1)
	// 3.Calculate output[i,j] = input[i,j] xor D[i]
	uint64_t C[5],D[5];
	int i;
    for (i = 0; i < 5; ++i) 
	{
        C[i]=0x0000000000000000;
        D[i]=0x0000000000000000;
        for (int j = 0; j < 5; ++j) 
		{
            C[i] = C[i]^input[i][j];
        }
    }

    for (i = 0; i < 5; ++i) 
	{
        D[i]=C[(i+4)%5]^Keccak_frot(C[(i + 1)%5],1);
    }

    for (int i = 0; i < 5; ++i) 
	{
        for (int j = 0; j < 5; ++j) 
		{
            output[i][j] = input[i][j]^D[i];
        }
    }
}

void Keccak_frho(uint64_t input[5][5], uint64_t output[5][5]) 
{
	//Input: @input 5*5 matrix of uint64_t, which store the matrix before rho transformation
	//       @output 5*5 matrix of uint64_t, which store the matrix after rho transformation
	// The 64 bits long string in the input 5*5 matrix will be moved by the specific matrix Rho_rot
	const int Rho_rot[5][5] = 
	{
        {0, 36, 3, 41, 18}, 
        {1, 44, 10, 45, 2}, 
        {62, 6, 43, 15, 61},
        {28, 55, 25, 21, 56},
        {27, 20, 39, 8, 14}
	};
    for (int i = 0; i < MAT; ++i) 
	{
        for (int j = 0; j < MAT; ++j) 
		{
            output[i][j] = Keccak_frot(input[i][j], Rho_rot[i][j]);
        }
    }
    
}
void Keccak_fpi(uint64_t input[5][5], uint64_t output[5][5]) 
{
	//Input: @input 5*5 matrix of uint64_t, which store the matrix before pi transformation
	//       @output 5*5 matrix of uint64_t, which store the matrix after pi transformation
	// The 64 bits long string in the input 5*5 matrix will be moved specifically, and here I do it one by one
	output[4][4]=input[0][4];
	output[3][3]=input[1][3];
	output[2][2]=input[2][2];
	output[1][1]=input[3][1];
	output[0][0]=input[4][0];
	
	
	output[4][2]=input[4][4];
	output[3][2]=input[3][3];
	output[2][2]=input[2][2];
	output[1][2]=input[1][1];
	output[0][2]=input[0][0];
	
	output[2][1]=input[4][2];
	output[2][4]=input[3][2];
	output[2][2]=input[2][2];
	output[2][0]=input[1][2];
	output[2][3]=input[0][2];
	
	output[1][4]=input[2][1];
	output[4][3]=input[2][4];
	output[2][2]=input[2][2];
	output[0][1]=input[2][0];
	output[3][0]=input[2][3];
	
	output[4][1]=input[1][4];
	output[3][4]=input[4][3];
	output[2][2]=input[2][2];
	output[1][0]=input[0][1];
	output[0][3]=input[3][0];


    output[1][3]=input[4][1];
	output[4][0]=input[3][4];
	output[2][2]=input[2][2];
	output[0][4]=input[1][0];
	output[3][1]=input[0][3];
	
	}
	
	
	
void Keccak_fchi(uint64_t input[5][5], uint64_t output[5][5]) 
{
	//Input: @input 5*5 matrix of uint64_t, which store the matrix before chi transformation
	//       @output 5*5 matrix of uint64_t, which store the matrix after chi transformation
	// The 64 bits long string in the input 5*5 matrix will be moved by the specific matrix Rho_rot
    for (int i = 0; i < 5; ++i) 
	{
        for (int j = 0; j < 5; ++j) 
		{
            output[i][j]=input[i][j]^((~input[(i+1)%5][j]&(input[(i+2)%5][j])));
        }
    }
}

void Keccak_fiota(uint64_t input[5][5], int round) 
{
	//Input: @input 5*5 matrix of uint64_t, which store the matrix before chi transformation
	//       @round the number of round it is in
	// Do xor to input[0][0] avoid symmetricity,each round the xor number is not the same
	const uint64_t iota[5][5]=
	{
		0x0000000000000001, 0x0000000000008082, 0x800000000000808A, 0x8000000080008000,
        0x000000000000808B, 0x0000000080000001, 0x8000000080008081, 0x8000000000008009, 
        0x000000000000008A, 0x0000000000000088, 0x0000000080008009, 0x000000008000000A,
        0x000000008000808B, 0x800000000000008B, 0x8000000000008089, 0x8000000000008003,
        0x8000000000008002, 0x8000000000000080, 0x000000000000800A, 0x800000008000000A,
        0x8000000080008081, 0x8000000000008080, 0x0000000080000001, 0x8000000080008008
	};
    input[0][0]=input[0][0]^iota[round];
}

void Keccak_cpy(uint64_t input[5][5], uint64_t output[5][5]) 
{
	//Input: @input 5*5 matrix of uint64_t, which store the matrix
	//       @output 5*5 matrix of uint64_t, which store the matrix copied from input
	for(int i=0;i<5;i++)
	{
		for(int j=0;j<5;j++)
		{
			output[i][j]=input[i][j];
		}
	}
}
void Keccak_f(uint64_t input[5][5], uint64_t output[5][5],int round) 
{
	//Input: @input 5*5 matrix of uint64_t, which store the matrix before Keccak transformation
	//       @output 5*5 matrix of uint64_t, which store the matrix after Keccak transformation
	//       @round the number of round it is in
	uint64_t buf_1[5][5],buf_2[5][5];
    Keccak_ftheta(input, buf_1);
    Keccak_frho(buf_1, buf_2);
    Keccak_fpi(buf_2,buf_1);
    Keccak_fchi(buf_1, buf_2);
    Keccak_fiota(buf_2, round);
    Keccak_cpy(buf_2,output);
}
int main()
{
	uint64_t x=0x2000000000000000;
	
	cout << hex << Keccak_frot(x,0);
	
	return 0;
}
