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

int main()
{
	uint64_t x=0x2000000000000000;
	
	cout << hex << Keccak_frot(x,0);
	
	return 0;
}
