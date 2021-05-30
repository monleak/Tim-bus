#include "cgraph/cgraph.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cgen/s2i.h"
#include "cgen/svector.h"
bn_tree_t s2i = NULL;
vector_t i2s = NULL;
int g_id = 0;

int get_save_id(char *key) 
{
	int res = s2i_value(s2i, key);    //search
	if(res != k_s2i_invalid) {		  //Nếu tìm thấy -> return
		return res;					
	} 					
	s2i_insert(s2i, key, g_id);		  //Nếu không tìm thấy -> insert
	svec_push_back(&i2s, key);
	res = g_id;
	++g_id;
	return res;
}
void xoa(char s[90],int vitrixoa)
{
	int n=strlen(s);//gán n= độ dài của chuỗi
    for(int i=vitrixoa;i<n;i++)//duyệt chuỗi từ vị trí cần xóa
     s[i]=s[i+1];
	s[n-1]='\0'; //kết thúc
}
void xoaspace(char *s)
{
	
	//Xoa dau cach o dau
	while(s[0]==' ')
	{
		xoa(s,0);
	}
	//Xoa dau cach o cuoi
	while(s[strlen(s)-1]==' ')
	{
		xoa(s,strlen(s)-1);
	}
}

int main()
{
	s2i = s2i_create();  //String to int
	i2s = gtv_create();	 //Int to string
	//Doc file
	FILE *data = fopen("../hanoibus.txt","r");
	char *kiemtra=NULL;
	cgraph_ivec_t edges = cgraph_ivec_create();
	do
	{
		//Lay vao 1 tuyen bus
		char *getline=(char*)calloc(10000,sizeof(char));
		kiemtra=fgets(getline,10000,data);
		if(strlen(getline)!=0) //Neu lay thanh cong -> phan tich du lieu
		{
			char *ngan_cach=strchr(getline,':'); //Tim ki tu : dau tien
			int i = ngan_cach-getline-1;

			//Lay ten xe (viet nguoc)
			char *tenxe=(char*)calloc(30,sizeof(char));  
			while(getline[i]!=' ')
			{
				tenxe[strlen(tenxe)]=getline[i];
				i--;
			}
			
			/*
			Cấu trúc 1 tuyến đường:
			Chiều đi tuyến 03B: Dải đỗ số 1 bến xe Giáp Bát - Tuyến 03 > Toyota Giải Phóng - 807 Giải Phóng > 649 Giải Phóng > 487 Giải Phóng > 175-177 Giải Phóng (Ngã 3 Lê Thanh Nghị) > 17 Giải Phóng - Biển báo số 1 > Đối diện 394 Lê Duẩn > Công viên Thống Nhất - Đối diện 326 Lê Duẩn > 1A Yết Kiêu > Cung VH Hữu Nghị Việt Xô - 91 Trần Hưng Đạo > 67 Trần Hưng Đạo > Trường Tiểu học Võ Thị Sáu - 35 Trần Hưng Đạo > 13A Trần Hưng Đạo > Cổng bệnh viện Quân y 108 - Đối diện 2E Trần Hưng Đạo > Đối diện khu tập thể 15C Trần Khánh Dư (chiều Trần Khánh Dư đi cầu Chương Dương) > Trước Phố Chương Dương Độ (Đối diện 198 Trần Quang Khải) > Cạnh Phố Hàm Tử Quan (Đối diện 162 Trần Quang Khải) > 80 Trần Nhật Duật  > 162 Nguyễn Văn Cừ - Biển báo số 1 > 358 - 360 Nguyễn Văn Cừ > 436 - 438 Nguyễn Văn Cừ > Hè trước nhà số 548 Nguyễn Văn Cừ > Phòng Công Chứng số 2 TPHN - 654 Nguyễn Văn Cừ > 18-20 Ngô Gia Tự > 170 - 172 Ngô Gia Tự - Đối diện chợ Ô Cách > 318 Ngô Gia Tự > 368 Ngô Gia Tự - Ngã 3 đường vào công An quận Long Biên > Đối diện Nhà K2 - KĐT Việt Hưng > Đối diện nhà B6 - KĐT Việt Hưng > Trường học 05 - KĐT Việt Hưng > Đối diện VP Vincom Village > Đối diện TTTM Vincom > Chợ Phúc Lợi > Trường THPT Phúc Lợi - Ngõ 115 Phúc Lợi > Đầu B: Phúc Lợi
			                   ^                                      ^                                    ^
			           danh_sach_diem_dung                     ngan_cach_diem_dung                 ngan_cach_diem_dung_next
			                                                 ngan_cach_diem_dung_prev              ngan_cach_diem_dung
			*/

			//Lay cac diem dung
			char *danh_sach_diem_dung=ngan_cach+1;
			char *ngan_cach_diem_dung_prev = danh_sach_diem_dung; //Dùng để lưu trữ ngăn cách điểm dừng trước đó, ban đầu nó sẽ là danh_sach_diem_dung
			do
			{
				char *ngan_cach_diem_dung=strchr(danh_sach_diem_dung,'>');
				if(ngan_cach_diem_dung==NULL) break;

				//Diem dung 1
				int do_dai_diem_dung1=ngan_cach_diem_dung-ngan_cach_diem_dung_prev;
				char *diem_dung1=(char*)calloc(do_dai_diem_dung1+2,sizeof(char));
				strncat(diem_dung1,danh_sach_diem_dung,do_dai_diem_dung1);
				xoaspace(diem_dung1);
				// printf("%s>",diem_dung1);
				int id1=get_save_id(diem_dung1);
				cgraph_ivec_push_back(&edges, id1);
				free(diem_dung1);

				danh_sach_diem_dung=ngan_cach_diem_dung+1;
				ngan_cach_diem_dung_prev = danh_sach_diem_dung;

				//Diem dung 2
				char *ngan_cach_diem_dung_next=strchr(danh_sach_diem_dung,'>');
				if(ngan_cach_diem_dung_next==NULL) ngan_cach_diem_dung_next=getline+strlen(getline);
				int do_dai_diem_dung2=ngan_cach_diem_dung_next-ngan_cach_diem_dung-1;
				char *diem_dung2=(char*)calloc(do_dai_diem_dung2+2,sizeof(char));
				strncat(diem_dung2,danh_sach_diem_dung,do_dai_diem_dung2);
				xoaspace(diem_dung2);
				// printf("%s\n",diem_dung2);
				int id2=get_save_id(diem_dung2);
				cgraph_ivec_push_back(&edges, id2);
				free(diem_dung2);
			}while(1);
			free(tenxe);
		}
		free(getline);
	}while(kiemtra!=NULL);

	//Do thi
	cgraph_t g=cgraph_create(edges, 0, true);
	// rewind(data);
	// // Doc lai file de luu cac canh
	// do
	// {
	// 	//Lay vao 1 tuyen bus
	// 	char *getline=(char*)calloc(10000,sizeof(char));
	// 	kiemtra=fgets(getline,10000,data);
	// 	if(strlen(getline)!=0) //Neu lay thanh cong -> phan tich du lieu
	// 	{
	// 		char *ngan_cach=strchr(getline,':'); //Tim ki tu : dau tien
	// 		int i = ngan_cach-getline-1;

	// 		//Lay ten xe (viet nguoc)
	// 		char *tenxe=(char*)calloc(30,sizeof(char));  
	// 		while(getline[i]!=' ')
	// 		{
	// 			tenxe[strlen(tenxe)]=getline[i];
	// 			i--;
	// 		}
			
			
	// 		Cấu trúc 1 tuyến đường:
	// 		Chiều đi tuyến 03B: Dải đỗ số 1 bến xe Giáp Bát - Tuyến 03 > Toyota Giải Phóng - 807 Giải Phóng > 649 Giải Phóng > 487 Giải Phóng > 175-177 Giải Phóng (Ngã 3 Lê Thanh Nghị) > 17 Giải Phóng - Biển báo số 1 > Đối diện 394 Lê Duẩn > Công viên Thống Nhất - Đối diện 326 Lê Duẩn > 1A Yết Kiêu > Cung VH Hữu Nghị Việt Xô - 91 Trần Hưng Đạo > 67 Trần Hưng Đạo > Trường Tiểu học Võ Thị Sáu - 35 Trần Hưng Đạo > 13A Trần Hưng Đạo > Cổng bệnh viện Quân y 108 - Đối diện 2E Trần Hưng Đạo > Đối diện khu tập thể 15C Trần Khánh Dư (chiều Trần Khánh Dư đi cầu Chương Dương) > Trước Phố Chương Dương Độ (Đối diện 198 Trần Quang Khải) > Cạnh Phố Hàm Tử Quan (Đối diện 162 Trần Quang Khải) > 80 Trần Nhật Duật  > 162 Nguyễn Văn Cừ - Biển báo số 1 > 358 - 360 Nguyễn Văn Cừ > 436 - 438 Nguyễn Văn Cừ > Hè trước nhà số 548 Nguyễn Văn Cừ > Phòng Công Chứng số 2 TPHN - 654 Nguyễn Văn Cừ > 18-20 Ngô Gia Tự > 170 - 172 Ngô Gia Tự - Đối diện chợ Ô Cách > 318 Ngô Gia Tự > 368 Ngô Gia Tự - Ngã 3 đường vào công An quận Long Biên > Đối diện Nhà K2 - KĐT Việt Hưng > Đối diện nhà B6 - KĐT Việt Hưng > Trường học 05 - KĐT Việt Hưng > Đối diện VP Vincom Village > Đối diện TTTM Vincom > Chợ Phúc Lợi > Trường THPT Phúc Lợi - Ngõ 115 Phúc Lợi > Đầu B: Phúc Lợi
	// 		                   ^                                      ^                                    ^
	// 		           danh_sach_diem_dung                     ngan_cach_diem_dung                 ngan_cach_diem_dung_next
	// 		                                                 ngan_cach_diem_dung_prev              ngan_cach_diem_dung
			

	// 		//Lay cac diem dung
	// 		char *danh_sach_diem_dung=ngan_cach+1;
	// 		char *ngan_cach_diem_dung_prev = danh_sach_diem_dung; //Dùng để lưu trữ ngăn cách điểm dừng trước đó, ban đầu nó sẽ là danh_sach_diem_dung
	// 		do
	// 		{
	// 			char *ngan_cach_diem_dung=strchr(danh_sach_diem_dung,'>');
	// 			if(ngan_cach_diem_dung==NULL) break;

	// 			//Diem dung 1
	// 			int do_dai_diem_dung1=ngan_cach_diem_dung-ngan_cach_diem_dung_prev;
	// 			char *diem_dung1=(char*)calloc(do_dai_diem_dung1+2,sizeof(char));
	// 			strncat(diem_dung1,danh_sach_diem_dung,do_dai_diem_dung1);
	// 			xoaspace(diem_dung1);
	// 			// printf("%s>",diem_dung1);
	// 			int id1=get_save_id(diem_dung1);
	// 			cgraph_ivec_push_back(&edges, id1);
	// 			free(diem_dung1);

	// 			danh_sach_diem_dung=ngan_cach_diem_dung+1;
	// 			ngan_cach_diem_dung_prev = danh_sach_diem_dung;

	// 			//Diem dung 2
	// 			char *ngan_cach_diem_dung_next=strchr(danh_sach_diem_dung,'>');
	// 			if(ngan_cach_diem_dung_next==NULL) ngan_cach_diem_dung_next=getline+strlen(getline);
	// 			int do_dai_diem_dung2=ngan_cach_diem_dung_next-ngan_cach_diem_dung-1;
	// 			char *diem_dung2=(char*)calloc(do_dai_diem_dung2+2,sizeof(char));
	// 			strncat(diem_dung2,danh_sach_diem_dung,do_dai_diem_dung2);
	// 			xoaspace(diem_dung2);
	// 			// printf("%s\n",diem_dung2);
	// 			int id2=get_save_id(diem_dung2);
	// 			cgraph_ivec_push_back(&edges, id2);
	// 			free(diem_dung2);
	// 		}while(1);
	// 		free(tenxe);
	// 	}
	// 	free(getline);
	// }while(kiemtra!=NULL);
	char from[30],to[30];
	while(1)
	{
		printf("==========TÌM ĐƯỜNG ĐI==========\n");
		printf("Nhập xuất phát = 0000 sẽ kết thúc chương trình\n");
		printf("xuất phát: ");gets(from);
		if(strcmp(from,"0000")==0) break;
		int idfrom=s2i_value(s2i, from);
		while(idfrom == k_s2i_invalid)
		{
			printf("Điểm dừng không tồn tại!\n");
			printf("xuất phát: ");gets(from);
			idfrom=s2i_value(s2i, from);
		}
		printf("Đến: ");gets(to);
		int idto=s2i_value(s2i, to);
		while(idto == k_s2i_invalid)
		{
			printf("Điểm dừng không tồn tại!\n");
			printf("Đến: ");gets(to);
			idto=s2i_value(s2i, to);
		}
		cgraph_ivec_t vpath = cgraph_ivec_create(),
                epath = cgraph_ivec_create();
		cgraph_get_shortest_path(g,&vpath,&epath,idfrom,idto,CGRAPH_OUT);
		cgraph_ivec_print(vpath);
		cgraph_ivec_print(epath);

		cgraph_ivec_free(&vpath);
		cgraph_ivec_free(&epath);
	}
	// int qw,as;
	// cgraph_edge(g, 3,(CGRAPH_INTEGER*)&qw,(CGRAPH_INTEGER*)&as);
	// printf("%s > %s\n",(char*)i2s[qw],(char*)i2s[as]);

	free(from);free(to);
	cgraph_ivec_free(&edges);
	cgraph_destroy(&g);
	fclose(data);
	return 0;
}