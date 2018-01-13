#include "syscall.h"

int main() {
	int arr[100];
	int i,j,count,temp,n,num,mult,result;
	char str[100];
	
	//Nhap so phan tu:
	do {
		PrintString("Nhap vao so luong mang : ");
		n = ReadInt();
		if (n > 100)
			PrintString("Ban da nhap sai\n");
	}while (n > 100);

	//Xu ly nhap phan tu
	count = 0;
	while(count < n) {
		i = 0;
		ReadString(str,1000);
		do {
			num = 0;
			mult = 1;

			while (('-' == (str[i])) || ((str[i]) == '+') || ((str[i]) == ' ')) {
				if (str[i] == '-')
					mult = mult * -1;
				i++;
			}
			while ((str[i] >= '0') && (str[i] <= '9')) {
				num = (num * 10) + ((str[i]) - '0');
				i++;
			}

			result = num * mult;
			arr[count] = result;
			count++;
		} while ((str[i]) != '\0');
	}

	//Thuat toan BubbleSort
	for (i = 0; i < n - 1; i++) {
		for (j = n - 1; j > i; j--) {
			if (arr[j] < arr[j - 1]) {
				temp = arr[j];
				arr[j] = arr[j - 1];
				arr[j - 1] = temp;
			}
		}
	}

	//Xuat mang
	PrintString("\nDay la chuoi tang dan: ");
	for (i = 0;i < n;i++)
	{	
		PrintInt(arr[i]);
		PrintChar(' ');
	}
	
	return 0;
}
