// Atmospheric neutrinos generator include deexcitation process, author : Jie Cheng ,email : chengjie@ihep.ac.cn
// usage:
// 1. just run the command below:
//      Atm.exe
// You may get some error output like that:
 /*
   Error in <TFile::TFile>: file /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre0/data/Generator/ProtonDecay/data//pd1.1.ghep.histo.root does not exist
     Can not find file: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre0/data/Generator/ProtonDecay/data//pd1.1.ghep.histo.root                                                                                   

  */
//    Once you meet the problem as showing above you should set the path to the GENIE file and TALYS file. You can run the command below:
//    Atm.exe -data_dir ~/junofs/juno_cenos7/data/Generator/AtmNC/data/ -talys_dir ~/junofs/juno_centos7/data/Generator/AtmNC/data/talys_sample/
// 2.  Argument interpretation
//  [-h ]  :list the all arguments you can choose.
//  [-seed SEED]: Specifies the random number seed.
//  [-n  nEvents]: Specifies the number of event.
//  [-o outputfilename] : Specifies the output file.
//  [-noDeex ] : if you don't want use Residual nuclear,you can use this.
//  [-data_dir /path/to/GenieFile/]:Specifies the path to GENIE file.


//  [-f mode] : Specifies the GENIE file name. Default is 0 which is represent for genie3.0.2_atm.root.
//   NC six nuclear models :
//   -f 2 : model1 : genie+talys , event rate = 3.59*1e-5/s/20kton
//   -f 3 : model3 : nuwro-1 + talys , event rate = 2.82586*1e-5/s/20kton
//   -f 4 : model2 : nuwro-2 + talys , event rate = 2.885*1e-5/s/20kton
//   -f 5 : model4 : nuwro-3 + talys , event rate = 3.341*1e-5/s/20kton
//   -f 6 : model5 : nuwro-4 + talys , event rate = 3.28265*1e-5/s/20kton
//   -f 7 : model6 : nuwro-5 + talys , event rate = 2.14872*1e-5/s/20kton
//   -f 0 : model1new : genie + talys , genie version 3.0.2, including NC and CC events
//  difference between models, please see https://juno.ihep.ac.cn/cgi-bin/Dev_DocDB/ShowDocument?docid=3210
//   CC nuclear model :
//   -f 1 : genie + talys , event rate = 8.0*1e-5/s
//   genie includes neutrino+C12 and neutrino+p interaction for NC and CC
//   nuwro only includes neutrino+C12


//  [-talys_dir /path/to/talysdata/] : Specifies the TALYS data. it is associated with deexcitation.
//  [-NC ] : Specifies the neutral current interaction of neutrino.
//  [-CC ] : Specifies the charge current interaction of neutrino. 
//  [-QEL] [-RES] [-COH/DFR] [-DIS] [-OTHERS]: These arguments are about different Physical reaction processes in different energy. You can specify any physical reaction you want to study. The default is we choose all the physical reaction.  
//  [-tran_ene_max TransferEnergyMax]: Transfer Energy means the total energy of final state particles (except neutrino) of neutrino interaction with nuclear. We can specify the maximum Transfer Energy to pre-selection event.  
//  [-tran_ene_min TransferEnergyMin]: We can specify the minimum transfer Energy to pre-selection event.

#include "deex.cc"
#include "Atm.h"

using namespace std;
using namespace CLHEP;


int main(int argc, char** argv)
{
  
  Params params;
  ProcessArgs(argc, argv, params);
  TString path_name = params.data_dir+params.GenieFilename;
  
  AtmEvent* aEvent = new AtmEvent(path_name);
  PreSelect *tool = new PreSelect(params);
  if( params.outFilename != NULL ){
       FILE* stream = fopen(params.outFilename, "w");
       if (stream == NULL) {
           printf("Please enetr a valid filename.\n");
           Usage();
           exit(0);
        }
        aEvent->SetStream(stream);    
     }
  PrintParams(params,aEvent->GetStream());

  gRandom->SetSeed(params.rseed);
  deex* DEProcess = new deex(params.talys_dir);  
 
  int nentries = aEvent->GetEntries();
 // int ievent = 0;
  for (int i = 0; i < params.nEvents; i++) {
   int  ievent = (int)(gRandom->Uniform() * (nentries - 1));
      //  std::cout<<"random1="<<gRandom->Uniform()<<std::endl;
        aEvent->LoadEvent(ievent);
        tool->Execute(*aEvent); 
    
    //std::cout<<"hdjh="<<tool->GetIsSelect()<<std::endl; 
    while(!tool->GetIsSelect()){
          ievent = (int)(gRandom->Uniform() * (nentries - 1));
        //  std::cout<<"random1="<<gRandom->Uniform()<<std::endl;
          aEvent->LoadEvent(ievent);
          tool->Execute(*aEvent);
       } 
    aEvent->AddPickEvtId(ievent);
    
    int PARS = aEvent->GetNpars();
    if(!params.doDeex) {
          aEvent->PrintTotalPars(PARS);
          aEvent->PrintNormPars();
          continue;
        }


    if (aEvent->GetIsNoIsoFlag()) {
      aEvent-> PrintTotalPars(PARS);
      aEvent->PrintNormPars();
    } else if (aEvent -> GetIsStableFlag() ) {
      aEvent->PrintTotalPars(PARS+1);
      aEvent->PrintNormPars();
      aEvent->PrintIos();
    } else {
      
      int rp = aEvent->GetIsoP();
      int rn = aEvent->GetIsoN();
      double exe = -1 ;
     
      DEProcess->SetParams(rp,rn);
      exe = DEProcess-> GetExE();
       

     // std::cout<<"exe="<<exe<<endl;
     // std::cout<<"random3="<<gRandom->Uniform()<<std::endl;
      if (exe > 0) {
        aEvent -> Info = DEProcess -> GetAfterDeexInfo();
        aEvent->PrintDeexInfo();
        aEvent->PrintNormPars();
      } else {
         aEvent->PrintTotalPars(PARS+1);
         aEvent->PrintNormPars();
         aEvent->PrintIos();  
      }
    }

  }
    
 
   if(params.user_output){
      aEvent->SetUserOutFileName(params.user_output);
      aEvent->OutputPickEvent();
   }

  delete DEProcess;  
  return 0;
}

void ProcessArgs(int argc, char** argv, Params& params)
{
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-seed") == 0) {
      i++;
      sscanf(argv[i], "%ld", &params.rseed);
    } else if (strcmp(argv[i], "-n") == 0) {
      i++;
      sscanf(argv[i], "%d", &params.nEvents);
    } else if (strcmp(argv[i], "-data_dir") == 0) {
      i++;
      char * data_dir = new char[strlen(argv[i]) + 1];
      strcpy(data_dir, argv[i]);
      params.data_dir = data_dir;
    } else if (strcmp(argv[i], "-talys_dir") == 0) {
      i++;
      char* talys_dir = new char[strlen(argv[i]) + 1];
      strcpy(talys_dir, argv[i]);
      params.talys_dir = talys_dir;
    } 

      else if (strcmp(argv[i], "-f") == 0) {
      i++;
      int flag = 0 ;
      sscanf(argv[i], "%d", &flag);
     // std::cout<<flag<<std::endl;
      if( flag < 0 ){
         std::cerr<<"invaild f value!!"<<std::endl;
         exit(-1);
       }
       params.GenieFilename = params.GenieFileMap[flag];
    }  
      
     else if (strcmp(argv[i], "-QEL") == 0){
         params.interType.push_back(1);
         params.interType.push_back(5);        
      } 
     else if (strcmp(argv[i], "-RES") == 0){
         params.interType.push_back(2);
     }
     else if (strcmp(argv[i], "-COH/DFR") == 0){
         params.interType.push_back(3);
     }
     else if (strcmp(argv[i], "-DIS") == 0){
         params.interType.push_back(4);
     }
   /*  else if (strcmp(argv[i], "-MEC") == 0){
         params.interType.push_back(5);
     }*/
     else if (strcmp(argv[i], "-OTHERS") == 0){
         params.interType.push_back(-1);
     }
     else if (strcmp(argv[i], "-NC") == 0){
         params.currentTag = 1;
     }
     
     else if (strcmp(argv[i], "-CC") == 0){
         params.currentTag = 2;
     }
    
     else if (strcmp(argv[i], "-tran_ene_max") == 0){
         i++;
         sscanf(argv[i], "%lf", &params.TransEMax);        
         if(params.TransEMax<0){
           std::cerr<<"Error:invalid value of maximum transfer energy !!"<<std::endl;
           exit(-1);
         }
      }
     else if (strcmp(argv[i], "-tran_ene_min") == 0){
         i++;
         sscanf(argv[i], "%lf", &params.TransEMin);
         if(params.TransEMax<0){
           std::cerr<<"Error:invalid value of minimum transfer energy !!"<<std::endl;
           exit(-1);
         }
      }
      else if(strcmp(argv[i], "-noDeex") == 0) {
          params.doDeex = false ;
      }
      else if(strcmp(argv[i], "-o") == 0) {
      i++;
      params.outFilename = new char[strlen(argv[i]) + 1];
      strcpy(params.outFilename, argv[i]);
    } 
      else if(strcmp(argv[i], "-user_output") == 0) {
      i++;
      params.user_output = new char[strlen(argv[i]) + 1];
      strcpy(params.user_output, argv[i]);
    }
   
     else if (strcmp(argv[i], "-h") == 0) {
      Usage();
      exit(0);
     }
   }
     if((params.TransEMax !=0) and ( params.TransEMax < params.TransEMin)){
       std::cerr<<"Error: params.TransEMax is smaller than params.TransEMin !!!"<<std::endl;
       exit(0);
    }

}

void PrintParams(Params a , FILE * stream){

  fprintf(stream, "# The configure of Atm Generator is following:\n");
  fprintf(stream, "# GENIE data is used from  %s.\n", (a.data_dir+a.GenieFilename).Data());
  fprintf(stream, "# TALYS data is used from  %s.\n", (a.talys_dir).Data());
  fprintf(stream, "# Random seed is %d.\n", a.rseed);
  fprintf(stream, "# Event number == %d.\n", a.nEvents);
  if(a.doDeex){
     fprintf(stream, "# invoke Deexcitation.\n");
  }
  else{
     fprintf(stream, "# not invoke Deexcitation.\n");
  }
  
    fprintf(stream,"# Interaction type we choose(1:QEL,2:RES,3:COH/DFR,4:DIS,5:MEC,-1:OTHERS):\n");
    if(!a.interType.size()){
        fprintf(stream, "# All interaction type we choose ");
      }
    else{
         fprintf(stream,"# ");
         std::vector<int>::iterator it;
         for(it = a.interType.begin();it!= a.interType.end();it++){
               fprintf(stream,"%d, ",(*it));
            }
         fprintf(stream,"\n");
        }
 
    if(!a.currentTag){
        fprintf(stream,"# NC and CC we choose.\n");
        }
    else{
          if(a.currentTag==1){
          fprintf(stream,"# we chose only NC.\n");
          }
          if(a.currentTag==2){
          fprintf(stream,"# we chose only CC.\n");
          }
        }

    if(!a.TransEMax){
         fprintf(stream,"# we have no transfer energy cut!!\n");
     }
     else{
         fprintf(stream,"# transfer energy is in [%lf GeV, %lf GeV].\n",a.TransEMin,a.TransEMax);
     }

 

}


void Usage()
{
  cout << "eg. \"Atm.exe [-h help] [-seed seed] [-o outputfilename] [-n nevents] [-data_dir /path/to/GenieFile/] [-f mode] [-talys_dir /path/to/talysdata/] [-NC] [-CC] [-QEL] [-RES] [-COH/DFR] [-DIS] [-OTHERS] [-tran_ene_max TransferEnergyMax] [-tran_ene_min TransferEnergyMin]\" " << endl;
}

