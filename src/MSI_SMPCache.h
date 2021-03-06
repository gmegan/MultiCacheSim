#ifndef MULTICACHESIM_MSI_SMPCACHE_H
#define MULTICACHESIM_MSI_SMPCACHE_H

#include "CacheCore.h"
#include "SMPCache.h"
#include "MSI_SMPCacheState.h"
#include <vector>

class MSI_SMPCache : public SMPCache{

public:

  //The actual SESC cache object
  CacheGeneric<StateGeneric<> > *cache;

  /*First we define a couple of helper classes that 
   * carry data between methods in our main class*/
  class RemoteReadService{
  public:
    bool isShared;
    bool providedData;
  
    RemoteReadService(bool shrd, bool prov){
      isShared = shrd;
      providedData = prov;
    }
  
  };
  
  class InvalidateReply{
  /*This class isn't used, but i left it here so you
   * could add to it if you need to communicate
   * between caches in Invalidate Replies*/
  public:
    uint32_t empty;
  
    InvalidateReply(bool EMPTY){
      empty = EMPTY;
    }
  
  };

  //FIELDS
  //This cache's ID
  int CPUId;


  //METHODS
  //Constructor
  MSI_SMPCache(int cpuid, 
               std::vector<SMPCache * > * cacheVector, 
               int csize, 
               int cassoc, 
               int cbsize, 
               int caddressable, 
               const char * repPol, 
               bool cskew);

  virtual int getStateAsInt(unsigned long addr);

  //Readline performs a read, and uses readRemoteAction to 
  //check for data in other caches
  //SMPCache Interface Function
  virtual void readLine(uint32_t rdPC, uint32_t addr, 
			uint32_t& memrd, uint32_t& wrback);
  virtual MSI_SMPCache::RemoteReadService readRemoteAction(uint32_t addr);

  //Writeline performs a write, and uses writeRemoteAction
  //to check for data in other caches
  //SMPCache Interface Function
  virtual void writeLine(uint32_t wrPC, uint32_t addr, uint32_t& wrback);
  virtual MSI_SMPCache::InvalidateReply writeRemoteAction(uint32_t addr);
 
  //Fill line touches cache state, bringing addr's block in, and setting its state to msi_state 
  //SMPCache Interface Function
  virtual void fillLine(uint32_t addr, uint32_t msi_state, uint32_t& wrback);

  virtual char *Identify();
 
  //Dump the stats for this cache to outFile
  //virtual void dumpStatsToFile(FILE* outFile);

  //Destructor
  ~MSI_SMPCache();

};


extern "C" {

  static SMPCache *MSI_SMPCache_Create(int num, 
				       std::vector<SMPCache*> *cvec, 
				       int csize, int casso, 
				       int bs, int addrble, 
				       const char *repl, bool skw)
  {
    return new MSI_SMPCache(num,cvec,csize,casso,bs,addrble,repl,skw);
  }

};

#endif
