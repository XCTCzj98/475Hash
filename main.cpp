#include <bits/stdc++.h>
#include <stdint.h>
#include <string>
#include <cstdlib>

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
    for (int i = 0; i < 5; ++i)
	{
        for (int j = 0; j < 5; ++j)
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
	const uint64_t iota[]=
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

uint64_t* encrypt(int type, uint64_t * seq, int seq_length)
{
	//Input: @type: define the type of Keccak
	//       @seq: the array of uint8_t(but stored in uint64_t, the former 56 bits should alwways be 0),
	//       because when we padding, we padding uint8_t. Stored as uint64_t avoid type changing
	//       And because each input char is composed of 8bits, so input should be uint8_t
	//       @seq_length: The number of uint8_t in the array
	//Output: the array of uint8_t(but stored in uint64_t, the former 56 bits should alwways be 0)
	int i,j,k;
	const int b=200;
	int output_length=type/8;
    int r, c; //in Byte
    if (type == 224) c = 56;
	else if (type == 256) c = 64;
	else if (type == 384) c = 96;
	else if (type == 512) c = 128;
	else {
	cout<<"No type"<<endl;
        return NULL;
    }

    r=b-c;

    int zero = 0;//the number of 0x00 at the tail
    if ((seq_length + 1)%r!=0) {
        zero=r-(seq_length+1)%r;//1 for 0x0000000000000006
    }
    int block = (seq_length+1+zero)/r;//How many blocks of length r
    uint64_t* padded_seq = new uint64_t[seq_length+1+zero];// the finally added sequence
    for(i=0;i<seq_length;i++)
    {
    	padded_seq[i]=seq[i];
	}

    padded_seq[seq_length]=0x0000000000000006;
    for(i=0;i<zero;i++)
    {
    	padded_seq[i+seq_length+1]=0x0000000000000000;
	}
    padded_seq[seq_length+zero]=padded_seq[seq_length+zero]|0x0000000000000080;

    uint64_t output[5][5];

    for(i=0;i<5;i++)
    {
    	for(j=0;j<5;j++)
    	{
    		output[i][j]=0x0000000000000000;
		}
	}

    for (i = 0; i < block; ++i)
	{
        int position = 0;
        //totally the bumber of block X absorbed
        for (j=i*r; j<(i+1)*r;j=j+8)
		{
			//j=j+8 because 1 uint_64 is composed of 8 uint_8
			// we need to reget the number in uint_64 from uint_8
            uint64_t t = 0x0000000000000000;
            for (k=j+7;k>=j;--k)
			{
				t=t<<8;
                t =t|padded_seq[k];
            }
            output[position%5][position/5] = output[position%5][position/5]^t;
            position++;
        }
        int num_round;
        uint64_t output_buf[5][5];
        for (num_round=0;num_round<24;num_round++)
		{
			//do 24 rounds of Keccak_f
            Keccak_f(output,output_buf,num_round);
            Keccak_cpy(output_buf,output);
        }
    }

    uint64_t* result=new uint64_t[output_length];

    for (i=0;i<output_length;)
	{
		//As in correct type, the length of output is times of 64 and of course time of 8
		//In each step we output one lane as 64bits=8*8bits
        for (j=0;j<8;j++)
		{
			//Only consider the last 2
            result[i]=output[(i/8)%5][(i/8)/5]&0x00000000000000ff;
            output[(i/8)%5][(i/8)/5]=output[(i/8)%5][(i/8)/5]>>8;
            i++;
        }
    }
    return result;


}

int main()
{
    // read the input
	cout << "Please input the security level of Keccak: " << endl;
	int type = 0;
	cin >> type;
	cout << "Please input the message: " << endl;
	string message;
	cin >> message;

	// turn the input into array of numbers
	uint64_t seq[message.length()];
	for (int i = 0; i < message.length(); i++) {
        int a = (int) message[i];
        seq[i] = a;
	}

    // encrypt
    uint64_t *result;
    result = encrypt(type, seq, message.length());

    //output in hex
    //the size of the output is security level / 8
    for (int i = 0; i < type/8; i ++) {
        cout << hex << result[i];
    }
    cout << endl;

	return 0;
}
