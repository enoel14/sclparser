#include <iostream>
#include <SCL.hxx>
#include <SCL_IED.hxx>

using namespace std;

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
   auto daTemplateList = scl->DataTypeTemplates().get().DAType();

   for (auto bda = bdaList.begin(); bda != bdaList.end(); bda++) {
      string name = bda->name();
      string btype = bda->bType();

      path.push_back(name);
      print_path(path,fc);

      if (btype == "Struct") {

         if (bda->type().present()) {
            string type = bda->type().get();

            auto datype = findDAType(daTemplateList, type);
            if (datype != daTemplateList.end()) {

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
   auto daTemplateList = scl->DataTypeTemplates().get().DAType();

   for (auto da = daList.begin(); da != daList.end(); da++) {
      string name = da->name();
      string btype = da->bType();
      int count = da->count();
      string fc = da->fc();
      
      path.push_back(name);
      print_path(path,fc);

      if (da->type().present()) {
         string type = da->type().get();

         auto datype = findDAType(daTemplateList, type);
         if (datype != daTemplateList.end()) {

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
   auto doTemplateList = scl->DataTypeTemplates().get().DOType();

   for (auto dot = sdoList.begin(); dot != sdoList.end(); dot++) {
      string name = dot->name();
      string type = dot->type();
      
      path.push_back(name);
      print_path(path);

      auto dotype = findDOType(doTemplateList, type);
      if (dotype != doTemplateList.end()) {

         auto daList = dotype->DA();
         parseDA(scl, daList,path);
      }

      path.pop_back();
   }
}

void parseDO(auto_ptr<SCL::SCL>& scl, SCL::tLNodeType::DO_sequence & doList, vector<string> &path)
{
   auto doTemplateList = scl->DataTypeTemplates().get().DOType();

   for (auto dot = doList.begin(); dot != doList.end(); dot++) {
      string name = dot->name();
      string type = dot->type();

      path.push_back(name);

      print_path(path);

      auto dotype = findDOType(doTemplateList, type);
      if (dotype != doTemplateList.end()) {
         
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

void parseLN(auto_ptr<SCL::SCL> &scl, SCL::tAccessPoint::LN_sequence& lnList, vector<string> &path)
{
   auto lnodeTemplateList = scl->DataTypeTemplates().get().LNodeType();

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

      auto lnode = findLNodeType(lnodeTemplateList, lntype);

      if (lnode != lnodeTemplateList.end()) {
         auto doList = lnode->DO();
         parseDO(scl, doList, path);
      }

      //parse dataset
      auto dsList = ln->DataSet();
      auto ds = dsList.begin();
      for (; ds != dsList.end(); ds++) {
         std::cout << "+dataset: " << ds->name() << std::endl;
      }

      //parse report control block
      auto rpList = ln->ReportControl();
      auto rp = rpList.begin();
      for (; rp != rpList.end(); rp++) {
         std::cout << "+report: " << rp->name() << " dataset: " << rp->datSet() << std::endl;
      }

      path.pop_back();

   }
}

void parseLN0(auto_ptr<SCL::SCL>& scl, SCL::tLDevice::LN0_type& dev, vector<string>& path)
{
   string ln0Type = dev.lnType();
   string ln0Class = dev.lnClass();

   path.push_back(ln0Class);

   print_path(path);

   //parse init value
   auto doiList = dev.DOI();
   parseInitValue(scl, doiList);


   auto lnodeTemplateList = scl->DataTypeTemplates().get().LNodeType();
   auto lnode = findLNodeType(lnodeTemplateList, ln0Type);

   if (lnode != lnodeTemplateList.end()) {
      auto doList = lnode->DO();
      parseDO(scl, doList, path);

   }

   //parse dataset
   auto dsList = dev.DataSet();
   auto ds = dsList.begin();
   for (; ds != dsList.end(); ds++) {
      std::cout << "+dataset: " << ds->name() << std::endl;
   }

   //parse report control block
   auto rpList = dev.ReportControl();
   auto rp = rpList.begin();
   for (; rp != rpList.end(); rp++) {
      std::cout << "+report: " << rp->name() << " dataset: " << rp->datSet() << std::endl;
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
      auto_ptr<SCL::SCL> scl(SCL::SCL_(file_name, xml_schema::flags::dont_validate));
      
      //std::cout << scl->Header().id() << std::endl;
      //std::cout << scl->Header().nameStructure() << std::endl;

      auto it = scl->Communication()->SubNetwork().begin();
      for (; it != scl->Communication()->SubNetwork().end(); it++) {
         string n = it->name();

         for (auto i = it->ConnectedAP().begin(); i != it->ConnectedAP().end(); i++) {
            //std::cout << "apName : " << i->apName() << std::endl;
            //std::cout << "iedName: " << i->iedName() << std::endl;

            std::cout << "Address" << std::endl;
            for (auto it = i->Address().get().P().begin(); it != i->Address().get().P().end(); it++) {
               std::cout << "-" << it->type() << ": " << it->data() << std::endl;
            }

            for (auto it = i->GSE().begin(); it != i->GSE().end(); it++) {
               string ldinst = it->ldInst();
               string cbname = it->cbName();

               std::cout << "GSE: " << ldinst << ";" << cbname << std::endl;
               for (auto piter = it->Address().get().P().begin(); piter != it->Address().get().P().end(); piter++) {
                  std::cout << "-" << piter->type() << ": " << piter->data() << std::endl;
               }
            }
         }
      }

      for(auto ied = scl->IED().begin(); ied != scl->IED().end(); ied++){

         std::cout << "--------------DEVICE------------------" << std::endl;
         std::cout << "  name        : " << ied->name() << std::endl;
         std::cout << "  manufacturer:" << ied->manufacturer() << std::endl;
         std::cout << "  desc        : " << ied->desc() << std::endl;
         std::cout << "  type        : " << ied->type() << std::endl;

         for (auto ap = ied->AccessPoint().begin() ; ap != ied->AccessPoint().end(); ap++){

            auto devList = ap->Server().get().LDevice();
            auto dev = devList.begin();
            for (; dev != devList.end(); dev++) {

               string ldName = ied->name() + dev->inst();

               vector<string> path;

               std::cout << "LD: " << ldName << std::endl;

               path.push_back(ldName);

               auto ln0 = dev->LN0();
               parseLN0(scl, ln0, path);

               auto lnList = dev->LN();
               parseLN(scl,lnList,path);

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
