#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include <cstring>
#include <stdio.h>
#include <unordered_map>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
struct sinode
{
char filename[20];
int filesize;
int blocknos[5];
};
struct fileobj
{
    int mode;
    int ino;
    int fd;
    int bno;

};
void mount_funs(string s);
void create_file();
void open_file();
void write_file();
void read_file();
void list_opened_files();
void list_files();
void close_file();
void delete_file();
void append_file();
void unmount_d(string s);
vector<string> disk_names;
unordered_map<string, fileobj> fdetails;
unordered_map<string,unordered_map<string, fileobj>> fulld;
long long int dsize=262144;
long long int dsize1=524288000;
long long int bsize=4096;
int nofblocks=dsize/bsize;
int inode_size=(int)sizeof(struct sinode);
int nofinode_blocks=3;
int nofdata_blocks=nofblocks-3-nofinode_blocks;
int noofinodes=(nofinode_blocks*bsize)/inode_size;
unordered_map<string, fileobj>::iterator itr;
fstream f1;
int fdNo=1;
char inodebm[100];
char databm[100];
//********create disk****************
int create_disk(string s)
{
fstream f;
char a[100];
strcpy(a,s.c_str());
f.open(a,ios::out);
if(!f)return 0;
else
{
//fseek(f1,200,SEEK_SET);
f.seekp(dsize1,ios::beg);
f.write(" ",1);
f.seekp(0,ios::beg);
//setting inode bits zero
f.seekp(bsize,ios::beg);
for(int i=0;i<nofdata_blocks;i++)
f<<"0";
//setting datanode bits zero
f.seekp(bsize*2,ios::beg);
for(int i=0;i<nofdata_blocks;i++)
f<<"0";
f.close();
return 1;
}
return 0;
}

//********************mount disk*****************
int mount_disk(string s)
{
//fstream f1;
char a[100];
char ch;
strcpy(a,s.c_str());
f1.open(a,ios::in| ios::out);
f1.seekg(bsize,ios::beg);
for(int i=0;i<nofdata_blocks;i++){
f1>>ch;
inodebm[i]=ch;
}
//reading databm
f1.seekp(bsize*2,ios::beg);
for(int i=0;i<nofdata_blocks;i++){
f1>>ch;
databm[i]=ch;
}
//reading object
for(int i=0;i<nofdata_blocks;i++)
{
    fileobj n;
    n.fd=-1;
    n.ino=-1;
    n.mode=-1;
    n.bno=-1;
 if(inodebm[i]==1)
 {
 f1.seekp((bsize*3+i*inode_size),ios::beg);
 char fn[20];
 char ch1;
 for(int k=0;k<20;k++)
  {
      f1>>ch;
      if(ch!=' ')
      {fn[k]=ch1;break;}
  }}
  //string f=fn;
  //fdetails[f]=n;

}
fdetails.clear();
unordered_map<string, fileobj> fx;
  if(fulld.find(s) == fulld.end())
   {
      cout<<"new mount sucesful"<<endl;
    fdetails=fx;}
  else
  {fdetails=fulld[s];cout<<"sucessful"<<endl;}

mount_funs(s);
return 0;
}
//****************functions**************
void mount_funs(string s)
{
    int x;
while(1==1)
{
    cout<<" 1-- create file"<<endl;
    cout<<" 2-- open file"<<endl;
    cout<<" 3-- read file"<<endl;
    cout<<" 4-- write file"<<endl;
    cout<<" 5-- append file"<<endl;
    cout<<" 6-- close file"<<endl;
    cout<<" 7-- delete file"<<endl;
    cout<<" 8-- list of file"<<endl;
    cout<<" 9-- list of opened file"<<endl;
    cout<<" 10-- unmount file"<<endl;
    cin>>x;
    if(x==1)
    {
      cout<<"create"<<endl;
      create_file();
    }
    else if(x==2)
    {
      cout<<"open"<<endl;
      open_file();
    }
    else if(x==3)
    {
      cout<<"read"<<endl;
      read_file();
    }
    else if(x==4)
    {
      cout<<"write"<<endl;
      write_file();
    }
    else if(x==5)
    {
      cout<<"append"<<endl;
      append_file();
    }
    else if(x==6)
    {
        cout<<"close"<<endl;
        close_file();
    }
    else if(x==7)
    {
      cout<<"delete"<<endl;
      delete_file();
    }
    else if(x==8)
    {
       cout<<"files"<<endl;
       list_files();
    }
    else if(x==9)
    {
      cout<<"opened files"<<endl;
      list_opened_files();
    }
    else if(x==10)
    {
     cout<<"unmount"<<endl;
     unmount_d(s);
     f1.close();
     break;
    }
    else
        cout<<"enter valid input"<<endl;

}

}
//**********create_file************
void create_file()
{   string s;
    cout<<"enter file name::";
    cin>>s;
 if(fdetails.find(s) != fdetails.end())cout<<"file already exists"<<endl;
 else{
        int flag=0;
   for(int i=0;i<nofblocks;i++)
   {
       if(inodebm[i]=='0')

       { f1.seekp(bsize*3+i*inode_size,ios::beg);
           for(int j=0;j<inode_size;j++)f1<<" ";
           flag=1;
           f1.seekp(bsize*3+i*inode_size,ios::beg);
           f1<<s;
           f1<<",";
           f1<<i;
           fileobj n;n.fd=-1;n.ino=-1;n.mode=-1;n.bno=-1;
           fdetails[s]=n;
           inodebm[i]='1';
           cout<<"file is created"<<endl;
           break;

       }
   }
   if(flag==0)cout<<"file is not created"<<endl;
 }

}
//*************open file********************
void open_file()
{
    string s;
    cout<<"enter file name::";
    cin>>s;
    if(fdetails.find(s) == fdetails.end())cout<<"file doesnot exists"<<endl;
    else{
        int m;
        cout<<"which mode(0-read,1-write,2-append)::";
        cin>>m;
        for (itr =fdetails.begin(); itr != fdetails.end(); ++itr)
        {
            if(itr->first==s)
            {
                if(itr->second.fd!=-1)cout<<"file is already opened"<<endl;
              else{
                itr->second.mode=m;
                itr->second.fd=fdNo;
                fdNo=fdNo+1;
                cout<<itr->first<<"  mode:"<<itr->second.mode<<" fd:"<<itr->second.fd<<endl;
                break;
              }
            }
        }
    }

}
//******************write file*********
void write_file()
{

  int fd;
  cout<<"Enter the file descriptor::";
    cin>>fd;
    int flag=0;
    for (itr =fdetails.begin(); itr != fdetails.end(); ++itr)
        {
         if(itr->second.fd==fd)
         {flag=1;
             if(itr->second.mode==0)cout<<"cannot write"<<endl;
             else{

          char ss[bsize];
          cin.getline (ss, bsize, '$');
          int ff=-1;
          for(int j=0;j<nofdata_blocks&&ff==-1;j++)
          {
              if(databm[j]=='0')
              {
               f1.seekg(bsize*(6+j),ios::beg);

               //cout<<f1.tellg()<<endl;
               f1<<ss;
               f1<<'$';
               f1.seekg(bsize*(6+j),ios::beg);

               databm[j]='1';
               //f1<<'$';
               ff=j;
              }
          }
          itr->second.bno=ff;
          //cout<<"block no::"<<itr->second.bno<<endl;
         }}

        }
        if(flag==0)cout<<"file descriptor does not exist"<<endl;




}
//*******************read file*************
void read_file()
{
 int fd;
  cout<<"Enter the file descriptor::";
    cin>>fd;
    int flag=0;
    for (itr =fdetails.begin(); itr != fdetails.end(); ++itr)
        {
         if(itr->second.fd==fd)
         {flag=1;
             if(itr->second.bno==-1)cout<<"no data in file"<<endl;
             else{

          int ff=itr->second.bno;
          //cout<<"block no::"<<ff<<endl;
          //f1.seekg(bsize*(6+ff),ios::beg);
          f1.seekg(bsize*(6+ff),ios::beg);
            char ch[bsize];
            f1.read(ch,bsize);
            for(int k=0;ch[k]!='$';k++)cout<<ch[k];
            cout<<endl;}
         }}
         if(flag==0)cout<<"file descriptor does not exist"<<endl;

}
//*******************append files*************
void append_file()
{
  int fd;
  cout<<"Enter the file descriptor::";
    cin>>fd;
    int flag=0;
    for (itr =fdetails.begin(); itr != fdetails.end(); ++itr)
        {
         if(itr->second.fd==fd)
         {
             if(itr->second.mode!=2)cout<<"cannot append"<<endl;
             else{
                    if(itr->second.bno==-1)cout<<"cannot append"<<endl;
             else{
          flag=1;
          int ff=itr->second.bno;
           f1.seekg(bsize*(6+ff),ios::beg);
            char ch[bsize];
            f1.read(ch,bsize);
            char ss[bsize];
          cin.getline (ss, bsize, '$');
          char f[bsize];
          for(int k=0;ch[k]!='$';k++)f[k]=ch[k];
          string sf1=f;
          string sf2=ss;
          string sf=sf1+" "+sf2+"$";
          f1.seekg(bsize*(6+ff),ios::beg);
          f1<<sf;
             }}
         }
        }


}
//**********************list files
void list_files()
{
    cout<<" files::";
 for (itr =fdetails.begin(); itr != fdetails.end(); ++itr)
 {
     cout<<itr->first<<"  ";
 }
cout<<endl;
}
void list_opened_files()
{
   cout<<"opened files::";
 for (itr =fdetails.begin(); itr != fdetails.end(); ++itr)
 {  if(itr->second.mode!=-1)
     cout<<itr->first<<"  ";
 }
cout<<endl;

}
//********************close file*************
void close_file()
{
int fd;
  cout<<"Enter the file descriptor::";
    cin>>fd;
    int flag=0;
    for (itr =fdetails.begin(); itr != fdetails.end(); ++itr)
        {
         if(itr->second.fd==fd)
         {
             itr->second.fd=-1;
             itr->second.mode=-1;
             flag=1;
             cout<<"file closed sucessfully"<<endl;
             break;
         }
        }
        if(flag==0)
            cout<<"file descriptor does not exist"<<endl;
}
//********************delete files
void delete_file()
{
  string s;
    cout<<"enter file name::";
    cin>>s;
    if(fdetails.find(s) == fdetails.end())cout<<"file doesnot exists"<<endl;
    else
    {
        for (itr =fdetails.begin(); itr != fdetails.end(); ++itr){
            if(itr->first==s)
            {   if(itr->second.ino!=-1)
                inodebm[itr->second.ino]='0';
                if(itr->second.bno!=-1)
                inodebm[itr->second.bno]='0';

            }
        }
        fdetails.erase(s);
       cout<<"file deleted sucessfully!!"<<endl;
    }

}
//***********************
void unmount_d(string s)
{
  for (itr =fdetails.begin(); itr != fdetails.end(); ++itr)
  {
    itr->second.fd=-1;
    itr->second.mode=-1;
  }
fulld[s]=fdetails;
f1.seekg(bsize,ios::beg);
for(int i=0;i<nofdata_blocks;i++)
f1<<inodebm[i];
//setting datanode bits zero
f1.seekg(bsize*2,ios::beg);
for(int i=0;i<nofdata_blocks;i++)
f1<<databm[i];

 fdetails.clear();
}
//************************
int main()
{
int x;
while(1==1)
{
	cout<<"press 1 -- creating disk"<<endl;
	cout<<"press 2 -- mount disk"<<endl;
	cout<<"press 3 -- exit application"<<endl;

	cin>>x;
	if(x==1)
	{

		cout<<"enter the disk name::";
		string s;
		cin>>s;
		int flag=0;
		for(int i=0;i<disk_names.size();i++)
		{
		if(disk_names[i]==s)
		flag=1;
		}
		if(flag==0)
			{
			int y=create_disk(s);
			if(y==0)cout<<"Error occured while creating disk"<<endl;
			else if(y==1){disk_names.push_back(s);cout<<"disk created sucessfully"<<endl;}
			}
		else
		cout<<"disk with the given name already exists"<<endl;
	}
	else if(x==2)
	{
		cout<<"mount disk"<<endl;
		cout<<"enter the disk name::";
		string s;
		cin>>s;
		int ffg=0;
		for(int ii=0;ii<disk_names.size();ii++)
         {
             if(disk_names[ii]==s)ffg=1;
         }
         if(ffg==1)
		mount_disk(s);
		else
            cout<<"disk not present"<<endl;
	}
	else if(x==3)
	{
		cout<<"exit ";
		exit(0);
	}
	else
		cout<<"enter valid input"<<endl;
}
return 0;
}
