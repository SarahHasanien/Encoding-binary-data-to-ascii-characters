#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <bitset>
#include <string>
using namespace std;
using namespace cv;

int convertImageToBinaryData(string imgName,string fileName,int threshold)
{
	ofstream orig(fileName); 
	Mat image = imread(imgName, CV_LOAD_IMAGE_GRAYSCALE);
	Mat bi = imread(imgName, CV_LOAD_IMAGE_GRAYSCALE);
	long long rows = image.rows;
	long long cols = image.cols;
	
	//filling the bilevel image
	for (long long r = 0; r < rows; r++)
	{
		for (long long c = 0; c < cols; c++)
		{
			if (image.at<uint8_t>(r, c)>threshold)
			{
				bi.at<uint8_t>(r, c) = 255;
				orig << 1;
			}
			else
			{
				bi.at<uint8_t>(r, c) = 0;
				orig << 0;
			}
		}
		orig << '\n';
	}
	imshow("Original Image", bi);
	return cols;
}
///////////////////////
void BinaryToAsciiEncoding(string BinaryFileName,string AsciiFileName)
{
	ifstream in(BinaryFileName);
	ofstream out(AsciiFileName);
	string row;
	while(in >> row)
	{
		istringstream inn(row);
		bitset<8> bs;
		while (inn >> bs)
		{
			out << char(bs.to_ulong());
		}
		out << '\n';
	}
	in.close();
	out.close();
}
////////////////////
void AsciiToBinaryDecoding(string AsciiFileName, string DecodedFileName, int ColSize)
{
	ifstream  f(AsciiFileName, ios::in | ios::binary);
	ofstream out(DecodedFileName);
	string temp = "";
	int x;
	int cnt = 0;
	string binaryString1, binaryString2;

	vector<char> c(istreambuf_iterator<char>(f), (istreambuf_iterator<char>()));
	for (int i = 0; i < c.size(); i++)
	{
		if (c[i+2] != '\n')
		{
			binaryString1 = "";
			binaryString2 = "";
			temp = "";
			unsigned char c2 = c[i];
			x = int(c2);
			while (x)
			{
				if (x % 2 == 0)
					binaryString1 += '0';
				else
					binaryString1 += '1';
				x /= 2;
			}
			if (binaryString1.size() != 8 )
				for (int i = 0; i < 8 - binaryString1.size(); i++)
					temp += '0';

			for (int j = binaryString1.size() - 1; j > -1; j--)
			{
				temp += binaryString1[j];
			}
			out << temp;
		}
		else
		{
			binaryString1 = "";
			binaryString2 = "";
			temp = "";
			unsigned char c2 = c[i];
			x = int(c2);
			while (x)
			{
				if (x % 2 == 0)
					binaryString1 += '0';
				else
					binaryString1 += '1';
				x /= 2;
			}
			if (binaryString1.size() != 8)
				for (int i = 0; i < 8 - binaryString1.size(); i++)
					temp += '0';

			for (int j = binaryString1.size() - 1; j > -1; j--)
			{
				temp += binaryString1[j];
			}
			//////////////
			if (ColSize % 8 != 0)
			{
				string temm = temp;
				temp = "";
				for (int i = 8 - (ColSize % 8); i < temm.size(); i++)
				{
					temp += temm[i];
				}
				out << temp;
				out << '\n';
				i += 2;
			}
			else
			{
				out << temp;
				out << '\n';
				i += 2;
			}
		}
	}
}

int main(int argc, char** argv)
{
	//convertImageToBinaryData takes image name, text file name and threshold.
	//It converts the image to 0s and 1s in the txt file 
	int cols = convertImageToBinaryData("file-page33.jpg","original.txt",127);


	//BinaryToAsciiEncoding takes a file original.txt contains 0s and 1s 
	//and encodes it in ascii characters in another file enc1.txt
	BinaryToAsciiEncoding("original.txt","enc1.txt");


	//AsciiToBinaryDecoding takes encoded file enc1.txt and decodes it to the original 0s and 1s file dec1.txt
	AsciiToBinaryDecoding("enc1.txt","dec1.txt", cols);

	//My algorithm ends here
	//The next lines are just to test if the decoded file is true =D
	Mat image = imread("file-page33.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	ifstream in("dec1.txt");
	string s,s2;
	long long j = 0;
	while (in >> s)
	{
		for (long long i = 0; i < s.size()-1; i++)
			if (s[i] == '0')
				image.at<uint8_t>(j,i) = 0;
			else
				image.at<uint8_t>(j,i) = 255;
		j++;
	}
	imshow("Decoded Image",image);
	in.close();
	ifstream in1("original.txt");
	ifstream in2("dec1.txt");
	int cc = 0;
	while (in1 >> s)
	{
		cc++;
		in2 >> s2;
		if (s != s2)
		{
			cout << "wrong decoding!";
			return 0;
		}
	}
	cout << "Done successfuly el7amdulilah =D\n";
	in1.close();
	in2.close();

	waitKey();
	return 0;
	}
