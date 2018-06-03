// ndn-dpe-linear-topo.cpp

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/ndnSIM/model/cs/ndn-content-store.hpp"

#include <sys/time.h>
#include "ns3/ndnSIM/utils/mem-usage.hpp"
#include "ns3/ndnSIM/utils/mem-usage.hpp"
#include "stdlib.h"
#include <string>
#include <ns3/vector.h>
#include "ns3/ndnSIM/NFD/core/algorithm.hpp"
#include <math.h>


using namespace std;

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("edpe_linear_print");

class print_pit{
public:
  print_pit() 
  :consumerHelper("ns3::ndn::ConsumerCbr"),
  success_detect(0),
  fail_detect(0),
  eta(0.0),
  error_detect(0),
  false_positive(0.0)
  {
  }


  void
  printStats();	

  int
  run(int argc, char* argv[]);

 private:
 	int16_t success_detect;
    int16_t fail_detect; 
	int16_t error_detect;
	double eta;
	double false_positive;
	//config user and attacker
    ndn::AppHelper consumerHelper;


};


void
print_pit::printStats()
{
  const ndn::Name prefix_user= "/dst/user";
  const ndn::Name prefix_attacker= "/dst/attcker";
  NodeContainer userNodes;
  userNodes.Add(Names::Find<Node>("NodeA"));   //legitimate user
  
  NodeContainer attackerNodes;   
  attackerNodes.Add(Names::Find<Node>("NodeB")); //attacker1

  //uint32_t counter[2]; 
  //counter[0]= Names::Find<Node>("Rtr1")->GetObject<ndn::L3Protocol>()->getForwarder()->getFib().findLongestPrefixMatch(prefix_user)->getCounter();
  //counter[1]= Names::Find<Node>("Rtr1")->GetObject<ndn::L3Protocol>()->getForwarder()->getFib().findLongestPrefixMatch(prefix_attacker)->getCounter();

  std::vector<ndn::Name>& MIL_check = Names::Find<Node>("Rtr1")->GetObject<ndn::L3Protocol>()->getForwarder()->getMIL();
 /* for(uint8_t it6=0;it6<MIL_check.size();it6++)
  {
    NS_LOG_DEBUG("MIL_check   = " << MIL_check[it6] <<" !!!"<<"/tExpired Counter  = "<<counter[it6]);
  };   */
  auto it1 = std::find(MIL_check.begin(),MIL_check.end(),prefix_attacker);
  auto it3 = std::find(MIL_check.begin(),MIL_check.end(),prefix_user);
  if(it1!=MIL_check.end())success_detect++;
  else fail_detect++;
  if(it3!=MIL_check.end())error_detect++;
  if((success_detect+fail_detect-1)!=0)eta = (double) success_detect/(success_detect+fail_detect-1);
  else eta=0;
  false_positive = (double) error_detect/(success_detect+fail_detect-1);
  NS_LOG_DEBUG("SUCCESS RATIO\t"<<eta << "\tFALSE POSITIVE\t"<<false_positive);
  Names::Find<Node>("Rtr1")->GetObject<ndn::L3Protocol>()->getForwarder()->clearMIL();
  MIL_check.clear();

  //config user and attacker
  //ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");//consumerHelper("ns3::ndn::ConsumerZipfMandelbrot"),


  //config user
  //consumerHelper.SetAttribute("NumberOfContents", StringValue("1000"));   //total content items
  consumerHelper.SetAttribute("LifeTime", StringValue("2s"));
  consumerHelper.SetPrefix("/dst/user");
  consumerHelper.SetAttribute("Frequency", StringValue(to_string(rand()%100+1))); // 1000 interests a second
  consumerHelper.Install(userNodes).Stop(Seconds(1.0));
  
  //config attacker1
  //consumerHelper.SetAttribute("NumberOfContents", StringValue("10000"));   //total content items
  consumerHelper.SetAttribute("LifeTime", StringValue("2s"));
  consumerHelper.SetPrefix("/dst/attcker");
  consumerHelper.SetAttribute("Frequency", StringValue(to_string(rand()%200+1))); // 150 interests a second
  consumerHelper.Install(attackerNodes).Stop(Seconds(1.0));

  Simulator::Schedule(Seconds(3.0), &print_pit::printStats, this);

}


int
print_pit::run(int argc, char* argv[])
{
  CommandLine cmd;
  cmd.Parse(argc, argv);

  AnnotatedTopologyReader topologyReader("", 25);
  topologyReader.SetFileName("src/ndnSIM/examples/topologies/linear-topo-edpe.txt");
  topologyReader.Read();

  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.SetOldContentStore("ns3::ndn::cs::Lru", "MaxSize", "10");
  //ndnHelper.SetOldContentStore("ns3::ndn::cs::Probability::Lru", "CacheProbability", "1");
  ndnHelper.setCsSize(10);
  ndnHelper.InstallAll();

  // Set BestRoute strategy
  ndn::StrategyChoiceHelper::InstallAll("/", "/localhost/nfd/strategy/best-route");

  // Installing global routing interface on all nodes
  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll();

  // Install NDN applications
  //std::string prefix_user = "/dst/user";
  //std::string prefix_attacker= "/dst/attcker";  

  // Getting containers for the consumer/producer
  Ptr<Node> producer = Names::Find<Node>("NodeC");
 
  //config producer
  ndn::AppHelper producerHelper("ns3::ndn::Producer");
  producerHelper.SetPrefix("/dst/user");
  producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
  producerHelper.Install(producer);

  // Add /prefix origins to ndn::GlobalRouter
  ndnGlobalRoutingHelper.AddOrigins("/dst/user", producer);
  ndnGlobalRoutingHelper.AddOrigins("/dst/attcker", producer);

  // Calculate and install FIBs
  ndn::GlobalRoutingHelper::CalculateRoutes();

  

 
  Simulator::Stop(Seconds(600.0));
  Simulator::Schedule(Seconds(0), &print_pit::printStats, this);
  Simulator::Run();
   
  Simulator::Destroy();
  
  

  return 0;
}

} // namespace ns3

int
main(int argc, char* argv[])
{
	ns3::print_pit print_pit;
	return print_pit.run(argc, argv);;
}

