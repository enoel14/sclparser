#include <iostream>
#include <SCL.hxx>
#include <SCL_IED.hxx>
#include <vector>
#include <unordered_map>
#include <map>
#include <list>

using namespace std;

vector<string> tags;


void print_path(vector<string>& path, string fc = "") 
{
   string path_name;

   int i = 0;
   path_name = path[i++];
   while(i<path.size()){
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

   //tags[path_name] = 0.0;
   tags.push_back(path_name);


   //cout << path_name << endl;
}

SCL::tDataTypeTemplates::LNodeType_const_iterator
findLNodeType(SCL::tDataTypeTemplates::LNodeType_sequence &l, string name)
{
   SCL::tDataTypeTemplates::LNodeType_const_iterator ln = l.begin();
   for (; ln != l.end(); ln++) {
      string id = ln->id();
      if (id == name) 
         return ln;
   }

   return l.end();
}

SCL::tDataTypeTemplates::DOType_const_iterator
findDOType(SCL::tDataTypeTemplates::DOType_sequence& l, string name)
{
   SCL::tDataTypeTemplates::DOType_const_iterator ln = l.begin();
   for (; ln != l.end(); ln++) {
      string id = ln->id();
      if (id == name) return ln;
   }

   return l.end();
}

SCL::tDataTypeTemplates::DAType_const_iterator
findDAType(SCL::tDataTypeTemplates::DAType_sequence& l, string name)
{
   SCL::tDataTypeTemplates::DAType_const_iterator ln = l.begin();
   for (; ln != l.end(); ln++) {
      string id = ln->id();
      if (id == name) return ln;
   }

   return l.end();
}

void parseBDA(auto_ptr<SCL::SCL>& scl, string fc, SCL::tDAType::BDA_sequence& bdaList, vector<string> &path)
{
   SCL::tDataTypeTemplates::DAType_sequence daTemplateList = scl->DataTypeTemplates().get().DAType();

   for (SCL::tDAType::BDA_const_iterator bda = bdaList.begin(); bda != bdaList.end(); bda++) {
      string name = bda->name();
      string btype = bda->bType();

      path.push_back(name);
      print_path(path,fc);
      //cout << "-----BDA: " << name << "[" << fc << "]" << endl;

      if (btype == "Struct") {

         if (bda->type().present()) {
            string type = bda->type().get();

            SCL::tDataTypeTemplates::DAType_const_iterator datype;
            datype = findDAType(daTemplateList, type);
            if (datype != daTemplateList.end()) {

               SCL::tDAType::BDA_sequence bdaList = datype->BDA();
               parseBDA(scl, fc, bdaList, path);
            }
         }
      }

      path.pop_back();
   }
}

void parseDA(auto_ptr<SCL::SCL>& scl, SCL::tDOType::DA_sequence & daList, vector<string> &path)
{
   SCL::tDataTypeTemplates::DAType_sequence daTemplateList = scl->DataTypeTemplates().get().DAType();

   for (SCL::tDOType::DA_const_iterator da = daList.begin(); da != daList.end(); da++) {
      string name = da->name();
      string btype = da->bType();
      int count = da->count();
      string fc = da->fc();
      
      path.push_back(name);
      print_path(path,fc);
      //cout << "----DA: " << name << "[" << fc << "]" << endl;

      if (da->type().present()) {
         string type = da->type().get();

         SCL::tDataTypeTemplates::DAType_const_iterator datype;
         datype = findDAType(daTemplateList, type);
         if (datype != daTemplateList.end()) {

            SCL::tDAType::BDA_sequence bdaList = datype->BDA();

            if (count > 0) {
               for (int i = 0; i < count; i++) {
                  
                  string vector = "[";
                  vector += to_string(i);
                  vector += "]";

                  path.push_back(vector);
                  print_path(path,fc);
                  //cout << "-----" << "[" << i << "]" << endl;

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
   SCL::tDataTypeTemplates::DOType_sequence doTemplateList = scl->DataTypeTemplates().get().DOType();

   for (SCL::tDOType::SDO_const_iterator dot = sdoList.begin(); dot != sdoList.end(); dot++) {
      string name = dot->name();
      string type = dot->type();
      
      path.push_back(name);
      //cout << "---SDO: " << name << ";" << type << endl;
      print_path(path);

      SCL::tDataTypeTemplates::DOType_const_iterator dotype;
      dotype = findDOType(doTemplateList, type);
      if (dotype != doTemplateList.end()) {

         SCL::tDOType::DA_sequence daList = dotype->DA();
         parseDA(scl, daList,path);
      }

      path.pop_back();
   }
}

void parseDO(auto_ptr<SCL::SCL>& scl, SCL::tLNodeType::DO_sequence & doList, vector<string> &path)
{
   SCL::tDataTypeTemplates::DOType_sequence doTemplateList = scl->DataTypeTemplates().get().DOType();

   for (SCL::tLNodeType::DO_const_iterator dot = doList.begin(); dot != doList.end(); dot++) {
      string name = dot->name();
      string type = dot->type();

      path.push_back(name);

      //cout << "--DO: " << name << endl; // ";" << type << endl;
      print_path(path);

      SCL::tDataTypeTemplates::DOType_const_iterator dotype;
      dotype = findDOType(doTemplateList, type);
      if (dotype != doTemplateList.end()) {
         
         if (!dotype->SDO().empty()) {
            SCL::tDOType::SDO_sequence sdoList = dotype->SDO();
            parseSDO(scl, sdoList,path);
         }

         SCL::tDOType::DA_sequence daList = dotype->DA();
         parseDA(scl,daList,path);
      }

      path.pop_back();

   }

}

void parseLN(auto_ptr<SCL::SCL> &scl, SCL::tAccessPoint::LN_sequence& lnList, vector<string> &path)
{
   SCL::tDataTypeTemplates::LNodeType_sequence lnodeTemplateList = scl->DataTypeTemplates().get().LNodeType();

   for (SCL::tAccessPoint::LN_const_iterator ln = lnList.begin(); ln != lnList.end(); ln++) {

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
      //SCL::tAnyLN::DOI_sequence doiList = ln->DOI();
      //for (SCL::tAnyLN::DOI_const_iterator doi = doiList.begin(); doi != doiList.end(); doi++) {
      //   doi->name();
      //}

      SCL::tDataTypeTemplates::LNodeType_const_iterator lnode;
      lnode = findLNodeType(lnodeTemplateList, lntype);

      if (lnode != lnodeTemplateList.end()) {
         SCL::tLNodeType::DO_sequence doList = lnode->DO();
         parseDO(scl, doList, path);
      }

      path.pop_back();

   }


}

int main(int argc, char* argv[])
{

   string file_name;

   if (argc != 2) {
      cout << "usage: 'sclparser scl_filename' " << endl;
      return 1;
   }
   else{
      file_name = string(argv[1]);
   }

   try
   {
      // Instantiate a SCL object from XML file
      auto_ptr<SCL::SCL> scl(SCL::SCL_(file_name, xml_schema::flags::dont_validate));

      //cout << scl->Header().id() << endl;
      //cout << scl->Header().nameStructure() << endl;

      for (SCL::tCommunication::SubNetwork_const_iterator it = scl->Communication()->SubNetwork().begin(); it != scl->Communication()->SubNetwork().end(); it++) {
         string n = it->name();

         for (SCL::tSubNetwork::ConnectedAP_const_iterator i = it->ConnectedAP().begin(); i != it->ConnectedAP().end(); i++) {
            //cout << "apName : " << i->apName() << endl;
            //cout << "iedName: " << i->iedName() << endl;

            cout << "Address" << endl;
            SCL::tAddress::P_const_iterator piter = i->Address().get().P().begin();
            for (; piter != i->Address().get().P().end(); piter++) {
               cout << "-" << piter->type() << ": " << piter->data() << endl;
            }

            for (SCL::tConnectedAP::GSE_const_iterator it = i->GSE().begin(); it != i->GSE().end(); it++) {
               string ldinst = it->ldInst();
               string cbname = it->cbName();

               cout << "GSE: " << ldinst << ";" << cbname << endl;
               for (SCL::tAddress::P_const_iterator piter = it->Address().get().P().begin(); piter != it->Address().get().P().end(); piter++) {
                  cout << "-" << piter->type() << ": " << piter->data() << endl;
               }
            }
         }
      }

      // iterate over all IEDs (<IED> elements) and print out some information about them
      for (SCL::SCL::IED_const_iterator i(scl->IED().begin()); i != scl->IED().end(); i++) {

         SCL::tIED ied = ((SCL::tIED) * i);

         cout << "--------------DEVICE------------------" << endl;
         cout << "  name        : " << ied.name() << endl;
         cout << "  manufacturer:" << ied.manufacturer() << endl;
         cout << "  desc        : " << ied.desc() << endl;
         cout << "  type        : " << ied.type() << endl;

         SCL::tIED::AccessPoint_const_iterator ap = SCL::tIED::AccessPoint_const_iterator(ied.AccessPoint().begin());
         for ( ; ap != ied.AccessPoint().end(); ap++)
         {
            //cout << "access point: " << ap->name() << endl;

            SCL::tServer::LDevice_sequence devList = ap->Server().get().LDevice();
            SCL::tServer::LDevice_const_iterator dev = devList.begin();
            for (; dev != devList.end(); dev++) {

               string ldName = ied.name() + dev->inst();

               vector<string> path;

               cout << "LD: " << ldName << endl;
               //cout << "ln type: " << dev->LN0().lnType() << " lnclass: " << dev->LN0().lnClass() << endl;

               //parse dataset
               SCL::tAnyLN::DataSet_sequence dsList = dev->LN0().DataSet();
               SCL::tAnyLN::DataSet_const_iterator ds = dsList.begin();
               for (; ds != dsList.end(); ds++) {
                  cout << "+dataset: " << ds->name() << endl;
               }

               //parse report control block
               SCL::tAnyLN::ReportControl_sequence rpList = dev->LN0().ReportControl();
               SCL::tAnyLN::ReportControl_const_iterator rp = rpList.begin();
               for (; rp != rpList.end(); rp++) {
                  cout << "+report: " << rp->name() << " dataset: " << rp->datSet() << endl;
               }
               
               SCL::tAccessPoint::LN_sequence lnList = dev->LN();

               path.push_back(ldName);
               parseLN(scl,lnList,path);
               path.pop_back();
          
            }
         }
      }
   }
   catch (const xml_schema::exception& e)
   {
      cerr << e << endl;
      return 1;
   }

   for (auto it = tags.begin(); it != tags.end(); it++) {
      string path = *it;
      cout << path << endl;
   }
}
