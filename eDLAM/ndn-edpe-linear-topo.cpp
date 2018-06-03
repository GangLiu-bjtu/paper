// ndn-dpe-linear-topo.cpp

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/ndnSIM/model/cs/ndn-content-store.hpp"

#include <sys/time.h>
#include "ns3/ndnSIM/utils/mem-usage.hpp"
#include "ns3/ndnSIM/utils/mem-usage.hpp"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE("edpe_linear_print");

class print_pit{
public:
  print_pit() 
  {
  }


  void
  printStats();	

  int
  run(int argc, char* argv[]);
};


void
print_pit::printStats()
{
  auto pitSize1 = Names::Find<Node>("Rtr1")->GetObject<ndn::L3Protocol>()->getForwarder()->getPit().size();
  //NS_LOG_DEBUG("Rtr1\t pitsize= "<<pitSize1<<"   !!!");
  
  auto pitSize2 = Names::Find<Node>("Rtr2")->GetObject<ndn::L3Protocol>()->getForwarder()->getPit().size();
  //NS_LOG_DEBUG("Rtr2\t pitsize= "<<pitSize2<<"   !!!");

  auto pitSize3 = Names::Find<Node>("Rtr3")->GetObject<ndn::L3Protocol>()->getForwarder()->getPit().size();
  NS_LOG_DEBUG("\t"<<pitSize1<<"\t"<<pitSize2<<"\t"<<pitSize3<<"\t");
  //NS_LOG_DEBUG("\n");
  
  Simulator::Schedule(Seconds(0.01), &print_pit::printStats, this);
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
  ndnHelper.SetOldContentStore("ns3::ndn::cs::Lru", "MaxSize", "10000");
  //ndnHelper.SetOldContentStore("ns3::ndn::cs::Probability::Lru", "CacheProbability", "1");
  ndnHelper.setCsSize(10000);
  ndnHelper.InstallAll();

  // Set BestRoute strategy
  ndn::StrategyChoiceHelper::InstallAll("/", "/localhost/nfd/strategy/best-route");

  // Installing global routing interface on all nodes
  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll();

  // Getting containers for the consumer/producer
  Ptr<Node> producer = Names::Find<Node>("NodeC");
  NodeContainer userNodes;
  userNodes.Add(Names::Find<Node>("NodeA"));   //legitimate user
  NodeContainer attackerNodes1;
  attackerNodes1.Add(Names::Find<Node>("NodeB"));   //attacker1
  NodeContainer attackerNodes2;
  attackerNodes2.Add(Names::Find<Node>("NodeD"));   //attacker2

  // Install NDN applications
  std::string prefix_user = "/dst/user";
  std::string prefix_attacker1= "/dst/attcker1";
  std::string prefix_attacker2= "/dst/attcker2";

  //config user and attacker
  ndn::AppHelper consumerHelper("ns3::ndn::ConsumerZipfMandelbrot");
  //config user
  consumerHelper.SetAttribute("NumberOfContents", StringValue("10000"));   //total content items
  consumerHelper.SetAttribute("LifeTime", StringValue("2s"));
  consumerHelper.SetPrefix(prefix_user);
  consumerHelper.SetAttribute("Frequency", StringValue("1000")); // 1000 interests a second
  consumerHelper.Install(userNodes);
  
  //config attacker1
  consumerHelper.SetAttribute("NumberOfContents", StringValue("10000"));   //total content items
  consumerHelper.SetAttribute("LifeTime", StringValue("2s"));
  consumerHelper.SetPrefix(prefix_attacker1);
  consumerHelper.SetAttribute("Frequency", StringValue("250")); // 150 interests a second
  consumerHelper.Install(attackerNodes1).Start(Seconds(30));

  //config attacker2
  consumerHelper.SetAttribute("NumberOfContents", StringValue("10000"));   //total content items
  consumerHelper.SetAttribute("LifeTime", StringValue("2s"));
  consumerHelper.SetPrefix(prefix_attacker2);
  consumerHelper.SetAttribute("Frequency", StringValue("250")); // 150 interests a second
  consumerHelper.Install(attackerNodes2).Start(Seconds(30));

//  ndn::AppHelper consumerHelper2("ns3::ndn::ConsumerWindow");
//  consumerHelper2.SetAttribute("PayloadSize", StringValue("240"));                //each interest size 26byte
//  consumerHelper2.Install(userNodes);
//  consumerHelper2.Install(attackerNodes);

  //config producer
  ndn::AppHelper producerHelper("ns3::ndn::Producer");
  producerHelper.SetPrefix(prefix_user);
  producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
  producerHelper.Install(producer);

  // Add /prefix origins to ndn::GlobalRouter
  ndnGlobalRoutingHelper.AddOrigins(prefix_user, producer);
  ndnGlobalRoutingHelper.AddOrigins(prefix_attacker1, producer);
  ndnGlobalRoutingHelper.AddOrigins(prefix_attacker2, producer);

  // Calculate and install FIBs
  ndn::GlobalRoutingHelper::CalculateRoutes();

  NS_LOG_DEBUG("TIME\t"<<"Rtr1\t"<<"Rtr2\t"<<"Rtr3\t"<<"\t");
  
  Simulator::Stop(Seconds(60));

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
	return print_pit.run(argc, argv);
}

