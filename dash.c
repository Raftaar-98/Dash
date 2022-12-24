///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Author1: Shishir Sunil Yalburgi  || Author 2: Uthama Kadengodlu        ////////////////////////////////////////
/// NET ID : SSY220000               || NET ID  : UXK210012                ////////////////////////////////////////
/// UTD ID : 2021665099              || UTD ID  :                          ////////////////////////////////////////
/// Version: 2.0.1                   ||                                    ////////////////////////////////////////
/// File   : dash.c                  ||                                    ////////////////////////////////////////
/// This file implements Dash shell                                        ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*****************************************************************************************************************/


/********************************************/
//////////////////////////////////////////////
///// Header File Inclusions /////////////////
//////////////////////////////////////////////
/********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/********************************************/
//////////////////////////////////////////////
// Global Variable           /////////////////
// declarations              /////////////////
//////////////////////////////////////////////
/********************************************/
ssize_t	    CommandLength = 0;
char       *CommandToken1,*CommandToken2;
char       *CommandData1,*CommandData2, *CommandData3, *PrlCmdFlag, *BuiltInCmdchk, *BuiltInCmdSvPtr=NULL;
size_t    CommandBufSize  = 0   ;
char    *CommandSavePtr1  = NULL,*CommandSavePtr2=NULL, *SearchPathSavePtr3 = NULL, *SrchPthScanner = NULL;
char     *mode            = NULL; 
char   BuiltInCommandFlg  = 0   ;
int    forkflag           = 0  ;
char *arg[10000];
char SearchPath[1000] = "/bin/";
char *Tok ;
int TokenLength=0;
int TokenLength2=0;
char error_message[30] = "An error has occurred\n"; 
char DashMsg[]  = "{dash>}";
int  AccessFlag = 1;
char TestPath[10000], TmpArray2 [1000] ;
int i=0,j=0;
char TmpArray[100000];
char strcounter[100];
int tmp=1;
char SearchPathStr[10000] = "/bin/";
FILE *fpointer;
int BatchCmdflag = 0;
char *FileInput;
char *FileInpSvPtr = NULL;
int LineCounter = 0 ;
/********************************************/
//////////////////////////////////////////////
// Macro                     /////////////////
// declarations              /////////////////
//////////////////////////////////////////////
/********************************************/
#define  ERR                               -1
#define  NO_ERR                             0
/********************************************/
//////////////////////////////////////////////
// Function                  /////////////////
// declarations              /////////////////
//////////////////////////////////////////////
/********************************************/

void ExecCommand(char *);
char resetPtr();
void ExecPrlCmd(char *);
void ExecbuiltInCommand(int);
void initialize();
int GetLineCount();
/********************************************/
//////////////////////////////////////////////
////////// Main Function /////////////////////
//////////////////////////////////////////////
/********************************************/

int main(int argc, char *argv[])
{

/********************************************/
//////////////////////////////////////////////
//////   Variable Declarations   /////////////
//////////////////////////////////////////////
/********************************************/
    if(argc ==2 )
    {
       BatchCmdflag = 1; 
       fpointer = fopen(argv[1],"r");
       if(fpointer == NULL)
       {
	    write(STDERR_FILENO, error_message, strlen(error_message));
            exit(0);
       }
       LineCounter = GetLineCount();
       fpointer = fopen(argv[1],"r");
    }
     

/********************************************/
//////////////////////////////////////////////
// Implementation begins from here ///////////
//////////////////////////////////////////////
/********************************************/

while(1)
{
    

    initialize();
    
    if(!strcmp(TmpArray,"cd"))
    {
     BuiltInCommandFlg = 1;
     ExecbuiltInCommand(BuiltInCommandFlg);
    }
    if(!strcmp(TmpArray,"path"))
    {
     BuiltInCommandFlg = 2;
     ExecbuiltInCommand(BuiltInCommandFlg);
    }
    if(!strcmp(TmpArray,"exit"))
    {
     BuiltInCommandFlg = 3;
     ExecbuiltInCommand(BuiltInCommandFlg);
    }
    
    
    
    if ((PrlCmdFlag==0 )&& !BuiltInCommandFlg)
        {
            ExecCommand(CommandData1);
        }
    else if((PrlCmdFlag!=0) && !BuiltInCommandFlg)
        {
           ExecPrlCmd(CommandData1);
           
        }
        else
        {
            ExecbuiltInCommand(BuiltInCommandFlg);
        }
 }		
}


void ExecPrlCmd(char *CommandData1)
{
    strcpy(SearchPathStr,SearchPath);
    if(CommandData1)
    {
        
        CommandToken1 = strtok_r(CommandData1,"&",&CommandSavePtr1);
        while(CommandToken1)
        {
            strcpy(SearchPath,SearchPathStr);
            SrchPthScanner = strtok_r(SearchPath," ",&SearchPathSavePtr3);
            CommandToken2 = strtok_r(CommandToken1," ",&CommandSavePtr2);
            while(SrchPthScanner)
            {
               strcpy(TestPath,SrchPthScanner);
               strcat(TestPath,CommandToken2);
               AccessFlag = access(TestPath,X_OK);
               if(AccessFlag==0)
               break;
               SrchPthScanner = strtok_r(NULL," ",&SearchPathSavePtr3);
            }
            i=0;
            arg[0]= TestPath;
          
           while(CommandToken2)
           {
               if(!AccessFlag)
              { i++;
               CommandToken2 = strtok_r(NULL," ",&CommandSavePtr2);
               if (CommandToken2)
			   {
			   CommandToken2 = strtok(CommandToken2,"\n");
			   arg[i] = CommandToken2;
			   }
			  }
              else 
              break;
           }
           arg [i] = NULL;
            
           if(!AccessFlag)
            {
                forkflag = fork();
                if(forkflag)
                {
                    
                    wait(NULL);
                }
                else
                {   
                    if(execv(TestPath,arg)== ERR)
                    {
                        write(STDERR_FILENO, error_message, strlen(error_message));
			 break;           
                    }
                    
                }
            }
            else
            {
                 write(STDERR_FILENO, error_message, strlen(error_message));
			break;    
            }
          CommandToken1 = strtok_r(NULL,"&",&CommandSavePtr1);
        
        }
    }
}
void ExecCommand(char *CommandToken1)
{
    strcpy(SearchPathStr,SearchPath);
    SrchPthScanner = strtok_r(SearchPath," ",&SearchPathSavePtr3);
    CommandToken2 = strtok_r(CommandToken1," ",&CommandSavePtr2);
    while(SrchPthScanner)
       {
            strcpy(TestPath,SrchPthScanner);
            strcat(TestPath,CommandToken2);
            AccessFlag = access((char*)TestPath,X_OK);
            if(AccessFlag==0)
            break;
            SrchPthScanner = strtok_r(NULL," ",&SearchPathSavePtr3);
        }
        if(AccessFlag)
        {
             write(STDERR_FILENO, error_message, strlen(error_message));
	     
        }
    i=0;
    
   
    while(CommandToken2)
       {
           i++;
           CommandToken2 = strtok_r(NULL," ",&CommandSavePtr2);
           if (CommandToken2)
           strcpy(TmpArray, CommandToken2);
           arg[i] = TmpArray;
           
       }
       arg [i] = NULL;
       arg [0]= TestPath;
       
       if(!access(TestPath,X_OK))
        {
            forkflag = fork();
            if(forkflag)
            {
                wait(NULL);
                
            }
            else
            {   
                if(execv(arg[0],arg)== ERR)
                    {
                        write(STDERR_FILENO, error_message, strlen(error_message));
			           
                    }
            }
        }
        else
            {
                 write(STDERR_FILENO, error_message, strlen(error_message));
			     
            }
      
}

void ExecbuiltInCommand (int BuiltInCommandFlg)
{
    switch(BuiltInCommandFlg)
    {
        case 1: 
        {
            BuiltInCommandFlg=0;
            CommandToken1 = strtok_r(NULL," ",&BuiltInCmdSvPtr);
            if(chdir(CommandToken1) == ERR)
            write(STDERR_FILENO, error_message, strlen(error_message));
            
        }
        break;
        case 2:
        {
            BuiltInCommandFlg=0;
            CommandToken1 = strtok_r(NULL," ",&BuiltInCmdSvPtr);
            strcpy(SearchPath,CommandToken1);
            while(CommandToken1)
            {
                CommandToken1 = strtok_r(NULL," ",&BuiltInCmdSvPtr);
                if(CommandToken1)
                {
                    strcat(SearchPath," ");
                    strcat(SearchPath,CommandToken1);
                    strcat(SearchPath," ");
                    
                }
            }
            strcpy(SearchPathStr,SearchPath);
            
            
        }
        break;
        case 3:
        {
            BuiltInCommandFlg=0;
            exit(0);
        }
        break;
        default: BuiltInCommandFlg=0;
        break;
    }
    
}
void initialize()
{
  	

  	if(BatchCmdflag == 0)
  	{
	  printf("%s ", DashMsg);
	  CommandLength = getline(&CommandData1,&CommandBufSize,stdin);
  	}
  	else
  	{
  	  if(LineCounter)
  	  {
	      
  	      CommandLength = getline(&CommandData1,&CommandBufSize,fpointer);
	      LineCounter --;
	     }
   
          else
          {
	     
	      exit(0);
	   }
  	  
  	}
	CommandData1  = strtok(CommandData1,"\n");
    PrlCmdFlag = strchr(CommandData1, '&');
    strcpy(TmpArray,CommandData1);
    BuiltInCmdchk = strtok_r(TmpArray," ",&BuiltInCmdSvPtr);
    strcpy(TmpArray,BuiltInCmdchk);
    BuiltInCommandFlg =0;  
    strcpy(SearchPath,SearchPathStr);
}
int GetLineCount()
{
  int result = 0;
  while(CommandLength>=0)
  {
   CommandLength = getline(&CommandData1,&CommandBufSize,fpointer);
   result ++;
  }
  result --;
  fclose(fpointer);
  return (result);

}




