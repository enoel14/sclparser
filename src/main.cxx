#include <iostream>
#include <SCL.hxx>
#include <SCL_IED.hxx>

using namespace std;

auto_ptr<SCL::SCL> __scl;
SCL::tDataTypeTemplates::LNodeType_sequence __lnodeTemplateList;
SCL::tDataTypeTemplates::DOType_sequence __doTemplateList;
SCL::tDataTypeTemplates::DAType_sequence __daTemplateList;
SCL::tDataTypeTemplates::EnumType_sequence __enumTemplateList;

void print_path(vector<string>& path, string fc = "") 
{
   string path_name;

   int i = 0;
   path_name = path[i++];
   while(i < path.size()){
      if (i == 1)
         path_name += "/";
      else
         path_name += "$";
      path_name += path[i++];
   }
   if (!fc.empty()) {
      path_name += "[";
      path_name += fc;
      path_name += "]";
   }

   std::cout << path_name << std::endl;
}

auto findLNodeType(SCL::tDataTypeTemplates::LNodeType_sequence &l, string name)
{
   for (auto ln = l.begin(); ln != l.end(); ln++) {
      string id = ln->id();
      if (id == name) 
         return ln;
   }

   return l.end();
}

auto findDOType(SCL::tDataTypeTemplates::DOType_sequence& l, string name)
{
   for (auto ln = l.begin(); ln != l.end(); ln++) {
      string id = ln->id();
      if (id == name) return ln;
   }

   return l.end();
}

auto findDAType(SCL::tDataTypeTemplates::DAType_sequence& l, string name)
{
   for (auto ln = l.begin(); ln != l.end(); ln++) {
      string id = ln->id();
      if (id == name) return ln;
   }

   return l.end();
}

void parseBDA(auto_ptr<SCL::SCL>& scl, string fc, SCL::tDAType::BDA_sequence& bdaList, vector<string> &path)
{
   for (auto bda = bdaList.begin(); bda != bdaList.end(); bda++) {
      string name = bda->name();
      string btype = bda->bType();

      path.push_back(name);
      print_path(path,fc);

      if (btype == "Struct") {

         if (bda->type().present()) {
            string type = bda->type().get();

            auto datype = findDAType(__daTemplateList, type);
            if (datype != __daTemplateList.end()) {

               auto bdaList = datype->BDA();
               parseBDA(scl, fc, bdaList, path);
            }
         }
      }

      path.pop_back();
   }
}

void parseDA(auto_ptr<SCL::SCL>& scl, SCL::tDOType::DA_sequence & daList, vector<string> &path)
{
   for (auto da = daList.begin(); da != daList.end(); da++) {
      string name = da->name();
      string btype = da->bType();
      int count = da->count();
      string fc = da->fc();
      
      path.push_back(name);
      print_path(path,fc);

      if (da->type().present()) {
         string type = da->type().get();

         auto datype = findDAType(__daTemplateList, type);
         if (datype != __daTemplateList.end()) {

            auto bdaList = datype->BDA();

            if (count > 0) {
               for (int i = 0; i < count; i++) {
                  
                  string vector = "[";
                  vector += to_string(i);
                  vector += "]";

                  path.push_back(vector);
                  print_path(path,fc);

                  parseBDA(scl, fc, bdaList, path);

                  path.pop_back();

               }

            }
            else {
               parseBDA(scl, fc, bdaList, path);
            }
         }
      }

      path.pop_back();
   }
}

void parseSDO(auto_ptr<SCL::SCL>& scl, SCL::tDOType::SDO_sequence& sdoList, vector<string> &path)
{
   for (auto dot = sdoList.begin(); dot != sdoList.end(); dot++) {
      string name = dot->name();
      string type = dot->type();
      
      path.push_back(name);
      print_path(path);

      auto dotype = findDOType(__doTemplateList, type);
      if (dotype != __doTemplateList.end()) {

         auto daList = dotype->DA();
         parseDA(scl, daList,path);
      }

      path.pop_back();
   }
}

void parseDO(auto_ptr<SCL::SCL>& scl, SCL::tLNodeType::DO_sequence & doList, vector<string> &path)
{
   for (auto dot = doList.begin(); dot != doList.end(); dot++) {
      string name = dot->name();
      string type = dot->type();

      path.push_back(name);

      print_path(path);

      auto dotype = findDOType(__doTemplateList, type);
      if (dotype != __doTemplateList.end()) {
         
         if (!dotype->SDO().empty()) {
            auto sdoList = dotype->SDO();
            parseSDO(scl, sdoList,path);
         }

         auto daList = dotype->DA();
         parseDA(scl,daList,path);
      }

      path.pop_back();

   }

}

void parseInitValue(auto_ptr<SCL::SCL>& scl, SCL::tAnyLN::DOI_sequence &doiList)
{
   for (auto doi = doiList.begin(); doi != doiList.end(); doi++) {
      string name = doi->name();
      std::cout << "  init: " << name << std::endl;
   }
}

void parseReport(SCL::tAnyLN::ReportControl_sequence& rpList)
{
   auto rp = rpList.begin();
   for (; rp != rpList.end(); rp++) {

      string name = rp->name();
      string datset = rp->datSet().get();
      int intgpd = rp->intgPd();
      string rptid = rp->rptID();

      int confrev = rp->confRev();
      bool buffered = rp->buffered();
      int buftm = rp->bufTime();

      uint8_t trgOps = 0;
      if (rp->TrgOps().present()) {
         if (rp->TrgOps().get().dchg()) trgOps |= 0x01;
         if (rp->TrgOps().get().qchg()) trgOps |= 0x02;
         if (rp->TrgOps().get().dupd()) trgOps |= 0x04;
         if (rp->TrgOps().get().period()) trgOps |= 0x08;
      }

      //if (intgpd > 0) trgOps |= 0x08;
      trgOps |= 0x10; //GI

      uint8_t options = 0;

      if (rp->OptFields().seqNum())			options |= 0x01;
      if (rp->OptFields().timeStamp())		options |= 0x02;
      if (rp->OptFields().reasonCode())	options |= 0x04;
      if (rp->OptFields().dataSet())		options |= 0x08;
      if (rp->OptFields().dataRef())		options |= 0x10;
      if (rp->OptFields().bufOvfl())		options |= 0x20;
      if (rp->OptFields().entryID())		options |= 0x40;
      if (rp->OptFields().configRef())		options |= 0x80;

      int max = 0;
      if (rp->RptEnabled().present()) {
         max = rp->RptEnabled().get().max();
      }

      string rptname = name;
      if (max > 1) {
         for (int i = 0; i < max; i++) {
            char tmp[4];
            sprintf(tmp, "%02d", i + 1);
            rptname = name + tmp;

            std::cout << "report: " << rptname << "; dataSet: " << datset << std::endl;
         }
      }
      else {
         std::cout << "report: " << rptname << "; dataSet: " << datset << std::endl;
      }
   }
}

bool getNetworkAddress(string ied)
{
   auto it = __scl->Communication()->SubNetwork().begin();
   for (; it != __scl->Communication()->SubNetwork().end(); it++) {
      string n = it->name();

      for (auto i = it->ConnectedAP().begin(); i != it->ConnectedAP().end(); i++) {
         string apName = i->apName();
         string iedName = i->iedName();

         if (iedName == ied) {
            std::cout << "  Address" << std::endl;
            for (auto it = i->Address().get().P().begin(); it != i->Address().get().P().end(); it++) {
               std::cout << "  -" << it->type() << ": " << it->data() << std::endl;
            }

            return true;
         }

      }
   }

   return false;
}

bool getGseNetworkAddress(SCL::tControlBlock::Address_optional& addr, string ied, string gsename)
{
   SCL::tCommunication::SubNetwork_const_iterator it = __scl->Communication()->SubNetwork().begin();

   for (; it != __scl->Communication()->SubNetwork().end(); it++) {
      string n = it->name();

      for (auto cap = it->ConnectedAP().begin(); cap != it->ConnectedAP().end(); cap++) {
         string apname = cap->apName();
         string iedname = cap->iedName();

         if (ied == iedname) {

            for (auto it = cap->GSE().begin(); it != cap->GSE().end(); it++) {
               string ldinst = it->ldInst();
               string cbname = it->cbName();

               if (gsename == cbname) {
                  if (it->Address().present()) {
                     addr = it->Address();
                     return true;
                  }
               }
            }
         }//
      }
   }

   return false;
}

void parseGSE(SCL::tLN0::GSEControl_sequence& gseList, string iedname)
{
   for (auto gse = gseList.begin(); gse != gseList.end(); gse++) {
      string name = gse->name();
      string appID = gse->appID();

      string datSet;
      if (gse->datSet().present())
         datSet = gse->datSet().get();

      int confRev = 0;
      if (gse->confRev().present())
         confRev = gse->confRev().get();

      SCL::tControlBlock::Address_optional address;
      if (getGseNetworkAddress(address, iedname, name)) {

         string dstAddr;
         uint16_t appId = 0;
         uint16_t vlanId = 0;
         uint8_t vlanPrio = 0;

         for (auto piter = address.get().P().begin(); piter != address.get().P().end(); piter++) {
            string param = piter->type();
            string data = piter->data();

            if (param == "MAC-Address") dstAddr = data.c_str();
            else if (param == "APPID")  appId = (int)strtol(data.c_str(), NULL, 16);  //if start with 0x strtol(hexstring, NULL, 0);//atol(PA.text().as_string());
            else if (param == "VLAN-ID") vlanId = atoi(data.c_str());
            else if (param == "VLAN-PRIORITY") vlanPrio = atoi(data.c_str());
         }

         std::cout << "GSE:: " << name << " Address: " << dstAddr << std::endl;
      }
   }
}

void parseSV(SCL::tLN0::SampledValueControl_sequence& svList)
{
   for (auto sv = svList.begin(); sv != svList.end(); sv++) {
      string name = sv->name();

      string datSet;
      if (sv->datSet().present())
         datSet = sv->datSet().get();

      string smvID = sv->smvID();

      uint32_t confRev = 0;
      if (sv->confRev().present())
         confRev = sv->confRev().get();

      uint16_t smpRate = sv->smpRate();
      bool isUnicast = sv->multicast();

      int optFlds = 0;
      uint8_t smpMod = 0;

      std::cout << "Sampled Value: " << name << " smvID: " << smvID << " smpRate: " << smpRate << std::endl;
   }
}

void parseDataset(SCL::tAnyLN::DataSet_sequence& dsList)
{
   auto ds = dsList.begin();
   for (; ds != dsList.end(); ds++) {
      string name = ds->name();

      std::cout << "dataset: " << name << std::endl;

      auto dsEntry = ds->FCDA().begin();
      for (; dsEntry != ds->FCDA().end(); dsEntry++) {
         string ldinst = dsEntry->ldInst().get();

         string prefix;

         if (dsEntry->prefix().present())
            prefix = dsEntry->prefix().get();

         string lnclass = dsEntry->lnClass().get();

         string lninst;
         if (dsEntry->lnInst().present())
            lninst = dsEntry->lnInst().get();

         string doname = dsEntry->doName().get();
         string fc = dsEntry->fc();

         string daname;
         if (dsEntry->daName().present()) {
            daname = dsEntry->daName().get();
         }

         string dsEntryName = ldinst + "/" + prefix + lnclass + lninst + "$" + fc + "$" + doname;
         if (!daname.empty()) {
            dsEntryName += "$";
            dsEntryName += daname;
         }

         std::cout << "dsEntry: " << dsEntryName << std::endl;

      }
   }
}

void parseLN(auto_ptr<SCL::SCL> &scl, SCL::tAccessPoint::LN_sequence& lnList, vector<string> &path)
{
   for (auto ln = lnList.begin(); ln != lnList.end(); ln++) {

      string lnclass = ln->lnClass();
      string lntype = ln->lnType();
      int lninst = ln->inst();

      string lnprefix;
      if(ln->prefix().present())
         lnprefix = ln->prefix().get();

      string lnname = lnprefix + lnclass + to_string(lninst);
      path.push_back(lnname);

      print_path(path);

      //parse init value
      auto doiList = ln->DOI();
      parseInitValue(scl, doiList);

      auto lnode = findLNodeType(__lnodeTemplateList, lntype);
      if (lnode != __lnodeTemplateList.end()) {
         auto doList = lnode->DO();
         parseDO(scl, doList, path);
      }

      //parse dataset
      auto dsList = ln->DataSet();
      parseDataset(dsList);

      //parse report control block
      auto rpList = ln->ReportControl();
      parseReport(rpList);

      path.pop_back();

   }
}

void parseLN0(auto_ptr<SCL::SCL>& scl, SCL::tServer::LDevice_const_iterator  dev, vector<string>& path, string iedname)
{
   auto ln0 = dev->LN0();

   string ln0Type = ln0.lnType();
   string ln0Class = ln0.lnClass();

   path.push_back(ln0Class);

   print_path(path);

   //parse init value
   auto doiList = ln0.DOI();
   parseInitValue(scl, doiList);


   auto lnodeTemplateList = scl->DataTypeTemplates().get().LNodeType();
   auto lnode = findLNodeType(lnodeTemplateList, ln0Type);

   if (lnode != lnodeTemplateList.end()) {
      auto doList = lnode->DO();
      parseDO(scl, doList, path);

   }

   //parse dataset
   auto dsList = ln0.DataSet();
   parseDataset(dsList);

   //parse report control block
   auto rpList = ln0.ReportControl();
   parseReport(rpList);

   //parsing GSE control block
   auto gseList = ln0.GSEControl();
   parseGSE(gseList, iedname);

   //parsing SV control block
   auto svList = ln0.SampledValueControl();
   parseSV(svList);

   //parsing setting group control block
   if (ln0.SettingControl().present()) {
      auto sg = ln0.SettingControl().get();
      int actSG = sg.actSG();
      int numOfSGs = sg.numOfSGs();
      
      std::cout << "Setting Group, actSG: " << actSG << std::endl;
   }

   path.pop_back();

}



int main(int argc, char* argv[])
{
   string file_name;

   if (argc != 2) {
      std::cout << "usage: 'sclparser scl_filename' " << std::endl;
      return 1;
   }
   else{
      file_name = string(argv[1]);
   }

   try
   {     
      __scl = SCL::SCL_(file_name, xml_schema::flags::dont_validate);
      __lnodeTemplateList = __scl->DataTypeTemplates().get().LNodeType();
      __doTemplateList = __scl->DataTypeTemplates().get().DOType();
      __daTemplateList = __scl->DataTypeTemplates().get().DAType();
      __enumTemplateList = __scl->DataTypeTemplates().get().EnumType();

      for(auto ied = __scl->IED().begin(); ied != __scl->IED().end(); ied++){

         std::cout << "--------------DEVICE------------------" << std::endl;
         std::cout << "  name        : " << ied->name() << std::endl;
         std::cout << "  manufacturer:" << ied->manufacturer() << std::endl;
         std::cout << "  desc        : " << ied->desc() << std::endl;
         std::cout << "  type        : " << ied->type() << std::endl;

         getNetworkAddress(ied->name());

         for (auto ap = ied->AccessPoint().begin() ; ap != ied->AccessPoint().end(); ap++){

            auto devList = ap->Server().get().LDevice();
            auto dev = devList.begin();
            for (; dev != devList.end(); dev++) {

               
               string ldName = ied->name() + dev->inst();

               vector<string> path;

               std::cout << "LD: " << ldName << std::endl;

               path.push_back(ldName);

               //auto ln0 = dev->LN0();
               parseLN0(__scl, dev, path, ied->name());

               auto lnList = dev->LN();
               parseLN(__scl,lnList,path);

               path.pop_back();
          
            }
         }
      }
   }
   catch (const xml_schema::exception& e)
   {
      std::cerr << e << std::endl;
      return 1;
   }

}
