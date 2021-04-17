#include <iostream>
#include <fstream>
#include <cassert>
#include <CLHEP/Vector/ThreeVector.h>
#include <CLHEP/Units/PhysicalConstants.h>
#include <TROOT.h>
#include <TRandom.h>
#include <vector>
#include <TSystem.h>
#include <TFile.h>
#include <TH1.h>
#include <TTree.h>
#include <cstdlib>
#include <TChain.h>
#include <TString.h>
#include "deex.h"
#include <map>
#include <algorithm>
using namespace std;
using namespace CLHEP;
void Usage();
class PreSelect;
struct Params{

   Params(){
     rseed = 0;
     nEvents = 1000000;
     outFilename=NULL;
     user_output=NULL;
     const char* path = getenv("JUNOTOP");
     data_dir = path;
     data_dir += "/data/Generator/Atm/data/";
     GenieFilename = "/genie3.0.2_atm.root";
      
     talys_dir = path; 
     talys_dir += "/data/Generator/AtmNC/data/talys_sample/";  
   
     interType.clear();
     currentTag = 0 ;
     TransEMax  = 0 ;
     TransEMin  = 0 ;
     GenieFileMap[0] = "/genie3.0.2_atm.root";
     GenieFileMap[1] = "/genie_ccdata.root";
     GenieFileMap[2] = "/genie_ncdata_02.root";
     GenieFileMap[3] = "/nuwro_nc_rfg990_02.root";
     GenieFileMap[4] = "/nuwro_nc_rfg1030_02.root";
     GenieFileMap[5] = "/nuwro_nc_rfg1350_02.root";
     GenieFileMap[6] = "/nuwro_nc_rfg990withTEM_02.root";
     GenieFileMap[7] = "/nuwro_nc_sf990_02.root";
     doDeex = true ;
  }

   long rseed;
   unsigned long int nEvents;
   TString data_dir;
   TString GenieFilename;
   TString talys_dir;
   char* outFilename;
   char* user_output;

   std::vector<int> interType;
   int currentTag;
   double TransEMax;
   double TransEMin;
   std::map<int,TString> GenieFileMap;
   bool doDeex;
//   FILE* stream ;
};

struct Data{

  Data(){
  
     evtID = 0 ;
     tPdg  = 0 ;
     tPx = 0. ;
     tPy  = 0.;
     tPz  = 0.;
     tRemovalE =0.;
     tE = 0.;
     tMass = 0.;

     pPdg = 0.;
     interType = 0;
     ctag = 0;
     pPx = 0;
     pPy = 0;
     pPz = 0;
     pEn = 0;


     m_isoPDG = 0 ;
     m_isoPdg = 0 ;
     m_isoTag = 0 ;
     m_isoPx  = 0.;
     m_isoPy  = 0.;
     m_isoPz  = 0.;
     m_isoE   = 0.;
     m_isoMass = 0.;
     m_isoEffMass = 0.;

     Npars = 0.;
     pdg[20] = {0};
     px[20] = {0};      // GeV
     py[20] = {0};      // GeV
     pz[20] = {0};      // GeV
     energy[20] = {0};  // GeV
     mass[20] = {0};    // GeV
     energyT = 0;

     Kpx = 0 ;
     Kpy = 0  ;
     Kpz = 0  ;
     Kenergy = 0;
}
  
  int evtID;
  int tPdg;
  double tPx;
  double tPy;
  double tPz;
  double tRemovalE;
  double tE;
  double tMass;
  
  int pPdg;
  int interType;
  int ctag;
  double pPx;
  double pPy;
  double pPz;
  double pEn;

  int m_isoPDG;
  int m_isoPdg;
  int m_isoTag;
  double m_isoPx;
  double m_isoPy;
  double m_isoPz;
  double m_isoE;
  double m_isoMass;
  double m_isoEffMass;


  int Npars;
  int pdg[20];
  double px[20];      // GeV
  double py[20];      // GeV
  double pz[20];      // GeV
  double energy[20];  // GeV
  double mass[20];    // GeV
  double energyT;

 
  double Kpx ;
  double Kpy  ;           
  double Kpz   ;          
  double Kenergy;         
};

class AtmEvent{
   public: 
   AtmEvent(TString path);
   ~AtmEvent();
   
   private:
     Data data;
   private:
     TFile* genieFile;
     TTree* geTree;

     TFile* useroutFile;
     TTree* useroutTree;
     TString useroutFilename;
   public:
     void SetUserOutFileName(TString s){ useroutFilename = s;} 
  public:
     void LoadReadFile(TString infile);
     void LoadEvent(int evtid);
     int GetNpars();
     int GetIsoP();
     int GetIsoN();
     bool GetIsStableFlag();
     bool GetIsNoIsoFlag();
  private:
     int entries;
     int iso_rp;
     int iso_rn;
     bool isstable;
     bool isnoiso;
  public:
     int GetEntries() { return entries; }
  public:
     AfterDeexInfo  Info;

  public:
     void PrintTotalPars( int num );
     void PrintNormPars();
     void PrintIos();
     void PrintDeexInfo();
  private:
     FILE* stream = stdout;
  public:
     void SetStream(FILE* a){ stream = a;}
     FILE * GetStream(){return stream;}
  private:
    std::map<int, double> pdg2mass;     

  public:
    Hep3Vector PointPv(double Energy, double Mass);


  public:
    void OutputPickEvent();
    void AddPickEvtId(int id){ PickEvtIdList.push_back(id); }
  private:
    std::vector<int> PickEvtIdList;

  friend PreSelect;
   
};

AtmEvent::AtmEvent(TString path){
    iso_rp = 0 ;
    iso_rn = 0 ;
    isstable = false ;
    isnoiso = false ;
    genieFile = NULL;
    geTree  = NULL;
    useroutFile = NULL;
    useroutFilename = "";
    useroutTree = NULL;
    entries = 0;
    PickEvtIdList.clear();
  
    pdg2mass[22] = 0;
    pdg2mass[2112] = 0.93957;
    pdg2mass[2212] = 0.93827;
    pdg2mass[1000010020] = 1.8756;
    pdg2mass[1000010030] = 2.8089;
    pdg2mass[1000020030] = 2.8084;
    pdg2mass[1000020040] = 3.7274;
    pdg2mass[1000030060] = 5.6015;
    pdg2mass[1000030070] = 6.5335;
    pdg2mass[1000030080] = 7.4708;
    pdg2mass[1000030090] = 8.4061;
    pdg2mass[1000040070] = 6.5344;
    pdg2mass[1000040080] = 7.4548;
    pdg2mass[1000040090] = 8.3925;
    pdg2mass[1000040100] = 9.3249;
    pdg2mass[1000050080] = 7.4728;
    pdg2mass[1000050090] = 8.3935;
    pdg2mass[1000050100] = 9.3244;
    pdg2mass[1000050110] = 10.2522;
    pdg2mass[1000060090] = 8.4100;
    pdg2mass[1000060100] = 9.3280;
    pdg2mass[1000060110] = 10.2542;
    pdg2mass[1000060120] = 11.1748;    

    LoadReadFile(path);  
 }

AtmEvent::~AtmEvent(){
  
   delete geTree;
   delete genieFile;

}

void AtmEvent::OutputPickEvent(){
     
     if(!useroutFilename){
       fprintf(stream, "plese input valid outFilename\n");
       exit(0);
     }
     if(!PickEvtIdList.size()){
       fprintf(stream, "no picked event!");
       return;
     }
     useroutFile = new TFile(useroutFilename, "RECREATE");
     useroutTree = new TTree("PickEvt", "PickEvt");
     useroutTree->Branch("evtID",&data.evtID,"evtID/I");
     useroutTree->Branch("tPdg", &data.tPdg,"tPdg/I");
     useroutTree->Branch("pPdg", &data.pPdg,"pPdg/I");
     useroutTree->Branch("pPx", &data.pPx,"pPx/D");
     useroutTree->Branch("pPy", &data.pPy,"pPy/D");
     useroutTree->Branch("pPz", &data.pPz,"pPz/D");
     useroutTree->Branch("pEn", &data.pEn,"pEn/D");
     useroutTree->Branch("interType", &data.interType,"interType/I");
     useroutTree->Branch("ctag", &data.ctag,"ctag/I");
     
     useroutTree->Branch("m_isoPdg", &data.m_isoPdg,"m_isoPdg/I");
     useroutTree->Branch("m_isoPx", &data.m_isoPx,"m_isoPx/D");
     useroutTree->Branch("m_isoPy", &data.m_isoPy,"m_isoPy/D");
     useroutTree->Branch("m_isoPz", &data.m_isoPz,"m_isoPz/D");
     useroutTree->Branch("m_isoMass", &data.m_isoMass,"m_isoMass/D"); 

     useroutTree->Branch("Npars", &data.Npars,"Npars/I");
     useroutTree->Branch("pdg", data.pdg,"pdg[Npars]/I");
     useroutTree->Branch("px", data.px,"px[Npars]/D");
     useroutTree->Branch("py", data.py,"py[Npars]/D");
     useroutTree->Branch("pz", data.pz,"pz[Npars]/D");
     useroutTree->Branch("energy", data.energy,"energy[Npars]/D");
     useroutTree->Branch("mass", data.mass,"mass[Npars]/D");   
     
     std::vector<int>::iterator it;
     for(it = PickEvtIdList.begin() ; it != PickEvtIdList.end() ; it++){
         geTree->GetEntry(*it);
         data.evtID = *it ; 
        // std::cout<<"evtID="<<data.evtID<<std::endl;
         useroutTree->Fill();
       } 
     useroutFile->cd();
     useroutTree->Write();
     useroutFile->Close();        
}

void AtmEvent::PrintTotalPars( int num ){
    fprintf(stream, "%d\n", num);    
}
void AtmEvent::PrintNormPars(){
    
     double epsilon = 1e-8;
     double ek = 0 ;
     for (int i = 0; i < data.Npars; i++) {
       ek = sqrt(data.px[i]*data.px[i]+data.py[i]*data.py[i]+data.pz[i]*data.pz[i]+data.mass[i]*data.mass[i])-data.mass[i];
       if( std::fabs(ek) < epsilon ){
           ek = 1e-3;
           Hep3Vector newPv = PointPv(ek, data.mass[i]);
           data.px[i] = newPv.x();
           data.py[i] = newPv.y();
           data.pz[i] = newPv.z(); 
        }
       fprintf(stream, "1\t%d 0 0 %f %f %f %f\n", data.pdg[i], data.px[i], data.py[i], data.pz[i], data.mass[i]);
     }
}
 
void AtmEvent::PrintIos(){
     fprintf(stream, "1\t%d 0 0 %f %f %f %f\n", data.m_isoPdg,data.m_isoPx,data.m_isoPy, data.m_isoPz, pdg2mass[data.m_isoPdg]);
}

void AtmEvent::PrintDeexInfo(){
   
    int count = 0;
    count = data.Npars + Info.NPars;
    if ( Info.ResidualZ >= 3 && Info.ResidualN >= 3)  {
          count = count + 1;
        //  fprintf(stream, "%d\n", count);
        //  fprintf(stream, "1\t%d 0 0 %f %f %f %f\n",Info.ResidualPdg , data.m_isoPx, data.m_isoPy,data.m_isoPz,pdg2mass[Info.ResidualPdg]);
       }
     fprintf(stream, "%d\n", count);
  /*  else{
          fprintf(stream, "%d\n", count);
        }
  */
    for (int k = 0; k < Info.NPars; k++) {
 
           double MASS = pdg2mass[Info.Pdg[k]];
           Hep3Vector pv = PointPv(Info.Energy[k], MASS);
           fprintf(stream, "1\t%d 0 0 %f %f %f %f\n", Info.Pdg[k], pv.x(), pv.y(), pv.z(), MASS);     
     }
    
    if ( Info.ResidualZ >= 3 && Info.ResidualN >= 3)  {
          fprintf(stream, "1\t%d 0 0 %f %f %f %f\n",Info.ResidualPdg , data.m_isoPx, data.m_isoPy,data.m_isoPz,pdg2mass[Info.ResidualPdg]);
       }



 }

Hep3Vector AtmEvent::PointPv(double Energy, double Mass ){

  double xdir = 0;
  double ydir = 0;
  double zdir = 0;
  double cost = 0.;
  double sint =0.;
  double phi = 0.;
  double Ekval = Energy*1e-3;
  double Pval = sqrt(pow(Ekval,2)+2*Mass*Ekval);

  cost = 1-2*gRandom->Uniform(0., 1.);
  sint = sqrt(1-cost*cost);
  phi = gRandom->Uniform(0, twopi);
  xdir = sint * cos(phi);
  ydir = sint * sin(phi);
  zdir = cost;

  double px = xdir * Pval;  // GeV
  double py = ydir * Pval;  // GeV
  double pz = zdir * Pval;  // GeV
  Hep3Vector Pv;
  Pv = Hep3Vector(px, py, pz);
  return Pv;

}



void AtmEvent::LoadReadFile(TString infile){

     genieFile = TFile::Open(infile, "read");
     if (!genieFile) {
        cout << "Can not find file: " << infile << endl;
        exit(-1);
     }
     geTree = (TTree*)genieFile->Get("particleT"); 
     entries = (int)geTree->GetEntries();
    
 // geTree->SetBranchAddress("evtID", &data.evtID);
  geTree->SetBranchAddress("tPdg", &data.tPdg);
  geTree->SetBranchAddress("pPdg", &data.pPdg);
  geTree->SetBranchAddress("pPx", &data.pPx);
  geTree->SetBranchAddress("pPy", &data.pPy);
  geTree->SetBranchAddress("pPz", &data.pPz);
  geTree->SetBranchAddress("pEn", &data.pEn);
  geTree->SetBranchAddress("interType", &data.interType);
  geTree->SetBranchAddress("ctag", &data.ctag);
//  geTree->SetBranchAddress("tMass", &data.tMass);
//  geTree->SetBranchAddress("tRemovalE", &data.tRemovalE);
//  geTree->SetBranchAddress("m_isoPDG", &data.m_isoPdg);
  geTree->SetBranchAddress("m_isoPdg", &data.m_isoPdg);
  geTree->SetBranchAddress("m_isoPx", &data.m_isoPx);
  geTree->SetBranchAddress("m_isoPy", &data.m_isoPy);
  geTree->SetBranchAddress("m_isoPz", &data.m_isoPz);
//  geTree->SetBranchAddress("m_isoE", &data.m_isoE);
  geTree->SetBranchAddress("m_isoMass", &data.m_isoMass);
//  geTree->SetBranchAddress("m_isoEffMass", &data.m_isoEffMass);  

  geTree->SetBranchAddress("Npars", &data.Npars);
  geTree->SetBranchAddress("pdg", data.pdg);
  geTree->SetBranchAddress("px", data.px);
  geTree->SetBranchAddress("py", data.py);
  geTree->SetBranchAddress("pz", data.pz);
  geTree->SetBranchAddress("energy", data.energy);
  geTree->SetBranchAddress("mass", data.mass);
//  geTree->SetBranchAddress("energyT", &data.energyT);
  
 /* geTree->SetBranchAddress("Kpx", &data.Kpx);
  geTree->SetBranchAddress("Kpy", &data.Kpy);
  geTree->SetBranchAddress("Kpz", &data.Kpz);
  geTree->SetBranchAddress("Kenergy", &data.Kenergy);*/
}

void AtmEvent::LoadEvent(int evtid){
     geTree->GetEntry(evtid);
    // std::cout<<"LoadEvent m_isoPDG="<<data.m_isoPdg<<std::endl;
     iso_rp = int((data.m_isoPdg - 1000000000) / 10000);
     iso_rn = int((data.m_isoPdg - 1000000000 - iso_rp * 10000) / 10) - iso_rp;

     if (data.m_isoPdg == 1000060120 || data.m_isoPdg == 1000030060 || iso_rp < 3 || iso_rn < 3 || iso_rp>6 || iso_rn>6) {
         isstable = true;
     }
     else{
         isstable = false;
     }
     if (data.m_isoPdg == 0 || iso_rp + iso_rn > 11) {
         isnoiso = true;
     }
     else{
         isnoiso = false;
     }                    
}

int AtmEvent::GetNpars(){
    return data.Npars;
}

int AtmEvent::GetIsoP()
{
     return iso_rp;
}

int AtmEvent::GetIsoN()
{
     return iso_rn ;
}

bool AtmEvent::GetIsStableFlag()
{
  return isstable ;
}

bool AtmEvent::GetIsNoIsoFlag(){
  return isnoiso ;
}


class PreSelect{

  public:
     PreSelect(Params & a);
    ~PreSelect();
  private:
     Params params;
     bool isselect;
  public:
     void Execute(AtmEvent & event );
     bool GetIsSelect(){return  isselect;}

}; 

PreSelect::PreSelect(Params & a){
   
     isselect = false ;
     params = a;
}
PreSelect::~PreSelect(){
  
}
 
void PreSelect::Execute(AtmEvent & event){
   
    if(params.interType.size()){
         std::vector<int>::iterator it; 
         it=std::find(params.interType.begin(),params.interType.end(),event.data.interType);
         if(it == params.interType.end())
         {
           isselect = false ;
           return;
         }        
      }

   if(params.TransEMax or params.TransEMin){
   
      double tfinalnuE = 0.;
      double entrans = 0.;
      if (event.data.ctag == 1) {
           for (int j = 0; j < event.data.Npars; j++) {
              if (event.data.pdg[j] == 12 || event.data.pdg[j] == -12 || event.data.pdg[j] == 14 || event.data.pdg[j] == -14) {
                tfinalnuE = sqrt(event.data.px[j] * event.data.px[j] + event.data.py[j] * event.data.py[j] + event.data.pz[j] * event.data.pz[j]);
                }
            }
             entrans = event.data.pEn - tfinalnuE;
         } else {
          entrans = event.data.pEn;
        }
    
      if( (entrans > params.TransEMax) or (entrans < params.TransEMin)){
          isselect = false;
          return;
        }
     }

      if(params.currentTag){

           if(params.currentTag != event.data.ctag ){
               isselect = false;
               return;
           }
         }
      isselect = true;
      return;
}


void ProcessArgs(int argc, char** argv, Params& params);
void PrintParams(Params a, FILE * stream);
