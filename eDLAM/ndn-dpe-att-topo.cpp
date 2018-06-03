// ndn-dpe-linear-topo.cpp

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/ndnSIM/model/cs/ndn-content-store.hpp"

#include <sys/time.h>
#include "ns3/ndnSIM/utils/mem-usage.hpp"
#include "ns3/ndnSIM/utils/mem-usage.hpp"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE("dpe_att_print");

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
  using ndn::Name;
  Name name("/dst/attcker/1");
  auto pitSize1 = Names::Find<Node>("bb-12659")->GetObject<ndn::L3Protocol>()->getForwarder()->getPit().size();
  //uint32_t rtr1_fib_counter = Names::Find<Node>("bb-13131")->GetObject<ndn::L3Protocol>()->getForwarder()->getFib().findExactMatch(name.getPrefix(2))->getCounter();
  //bool rtr1_fib_isattacker = Names::Find<Node>("bb-13131")->GetObject<ndn::L3Protocol>()->getForwarder()->getFib().findExactMatch(name.getPrefix(2))->get_isattacker();
  //NS_LOG_DEBUG("bb-12659\t pitsize= "<<pitSize1<<"\t fib.prefix= "<<name.getPrefix(2)<<"\t fib.counter= "<<rtr1_fib_counter<<"\t fib.isattacker= "<<rtr1_fib_isattacker);
  NS_LOG_DEBUG("bb-12659\t pitsize= "<< pitSize1 <<"    !");
  NS_LOG_DEBUG("\n");
  
  Simulator::Schedule(Seconds(0.01), &print_pit::printStats, this);
}


int
print_pit::run(int argc, char* argv[])
{
  CommandLine cmd;
  cmd.Parse(argc, argv);

  AnnotatedTopologyReader topologyReader("", 25);
  topologyReader.SetFileName("src/ndnSIM/examples/topologies/7018-r0.txt");
  topologyReader.Read();

  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.SetOldContentStore("ns3::ndn::cs::Lru", "MaxSize", "500");
  //ndnHelper.SetOldContentStore("ns3::ndn::cs::Probability::Lru", "CacheProbability", "1");
  ndnHelper.setCsSize(500);
  ndnHelper.InstallAll();

  // Set BestRoute strategy
  ndn::StrategyChoiceHelper::InstallAll("/", "/localhost/nfd/strategy/best-route");

  // Installing global routing interface on all nodes
  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll();

  // Getting containers for the consumer/producer
  Ptr<Node> producer = Names::Find<Node>("gw-12633");
  
  NodeContainer userNodes;
  std::string user_name[196]={"leaf-4626",	
"leaf-11242",	
"leaf-6868",	
"leaf-12512",	
"leaf-12480",	
"leaf-12483",	
"leaf-12486",	
"leaf-12452",	
"leaf-12651",	
"leaf-12473",	
"leaf-12478",	
"leaf-12451",	
"leaf-13118",	
"leaf-13120",	
"leaf-12472",	
"leaf-12471",	
"leaf-12479",	
"leaf-12484",	
"leaf-12488",	
"leaf-12481",	
"leaf-12482",	
"leaf-12485",	
"leaf-12492",	
"leaf-12489",	
"leaf-12490",	
"leaf-12491",	
"leaf-12493",	
"leaf-12494",	
"leaf-12495",	
"leaf-12540",	
"leaf-12497",	
"leaf-12496",	
"leaf-12513",	
"leaf-12498",	
"leaf-12509",	
"leaf-12510",	
"leaf-12511",	
"leaf-12528",	
"leaf-12618",	
"leaf-12623",	
"leaf-12673",	
"leaf-12872",	
"leaf-12508",	
"leaf-12507",	
"leaf-12619",	
"leaf-12873",	
"leaf-12514",	
"leaf-12516",	
"leaf-12515",	
"leaf-12523",	
"leaf-12950",	
"leaf-12520",	
"leaf-12522",	
"leaf-12551",	
"leaf-12529",	
"leaf-12538",	
"leaf-12678",	
"leaf-12552",	
"leaf-13092",	
"leaf-12595",	
"leaf-12559",	
"leaf-12561",	
"leaf-12562",	
"leaf-12563",	
"leaf-12565",	
"leaf-12566",	
"leaf-12568",	
"leaf-12572",	
"leaf-12575",	
"leaf-12570",	
"leaf-12571",	
"leaf-12573",	
"leaf-12579",	
"leaf-12577",	
"leaf-12583",	
"leaf-12581",	
"leaf-12584",	
"leaf-12614",	
"leaf-12587",	
"leaf-12604",	
"leaf-12606",	
"leaf-12622",	
"leaf-12729",	
"leaf-12730",	
"leaf-12783",	
"leaf-12788",	
"leaf-12946",	
"leaf-12947",	
"leaf-13080",	
"leaf-12601",	
"leaf-12616",	
"leaf-12617",	
"leaf-12696",	
"leaf-12707",	
"leaf-12708",	
"leaf-12709",	
"leaf-12776",	
"leaf-12885",	
"leaf-12952",	
"leaf-12789",	
"leaf-12621",	
"leaf-12620",	
"leaf-12625",	
"leaf-12624",	
"leaf-12626",	
"leaf-12627",	
"leaf-12653",	
"leaf-12655",	
"leaf-12643",	
"leaf-12646",	
"leaf-12636",	
"leaf-12635",	
"leaf-12642",	
"leaf-12639",	
"leaf-12638",	
"leaf-12648",	
"leaf-12640",	
"leaf-12650",	
"leaf-12644",	
"leaf-12647",	
"leaf-12656",	
"leaf-12649",	
"leaf-12672",	
"leaf-12724",	
"leaf-12706",	
"leaf-12705",	
"leaf-12675",	
"leaf-12879",	
"leaf-12880",	
"leaf-13082",	
"leaf-12935",	
"leaf-12680",	
"leaf-12676",	
"leaf-12674",	
"leaf-12677",	
"leaf-12697",	
"leaf-12690",	
"leaf-12687",	
"leaf-12683",	
"leaf-12681",	
"leaf-12682",	
"leaf-12688",	
"leaf-12686",	
"leaf-12685",	
"leaf-12684",	
"leaf-12689",	
"leaf-12699",	
"leaf-13052",	
"leaf-13053",	
"leaf-13054",	
"leaf-12700",	
"leaf-12698",	
"leaf-13086",	
"leaf-12701",	
"leaf-12703",	
"leaf-12715",	
"leaf-12716",	
"leaf-12717",	
"leaf-12723",	
"leaf-12718",	
"leaf-12719",	
"leaf-12720",	
"leaf-12721",	
"leaf-12722",	
"leaf-12726",	
"leaf-12725",	
"leaf-12876",	
"leaf-12877",	
"leaf-12908",	
"leaf-12731",	
"leaf-12734",	
"leaf-12741",	
"leaf-12743",	
"leaf-12738",	
"leaf-12740",	
"leaf-12739",	
"leaf-12746",	
"leaf-12919",	
"leaf-12742",	
"leaf-12747",	
"leaf-12744",	
"leaf-12745",	
"leaf-12748",	
"leaf-12763",	
"leaf-12773",	
"leaf-12951",	
"leaf-13057",	
"leaf-12762",	
"leaf-12960",	
"leaf-12777",	
"leaf-13112",	
"leaf-12782",	
"leaf-12775",	
"leaf-12774",	
"leaf-13116",	
"leaf-12781"};
  
  for(int64_t i=0;i<196;i++)
  {
    userNodes.Add(Names::Find<Node>(user_name[i]));   //legitimate user
  }
  

  NodeContainer attackerNodes;
  std::string attacker_name[100]={"leaf-13113",	
"leaf-12824",	
"leaf-12878",	
"leaf-12828",	
"leaf-12853",	
"leaf-12852",	
"leaf-12883",	
"leaf-12892",	
"leaf-12882",	
"leaf-12862",	
"leaf-12889",	
"leaf-12890",	
"leaf-12893",	
"leaf-12895",	
"leaf-12891",	
"leaf-12902",	
"leaf-12903",	
"leaf-12894",	
"leaf-12896",	
"leaf-12897",	
"leaf-12898",	
"leaf-12899",	
"leaf-12900",	
"leaf-12901",	
"leaf-12917",	
"leaf-12916",	
"leaf-12915",	
"leaf-12911",	
"leaf-13083",	
"leaf-13084",	
"leaf-13085",	
"leaf-13089",	
"leaf-12924",	
"leaf-12925",	
"leaf-12927",	
"leaf-12933",	
"leaf-12934",	
"leaf-13087",	
"leaf-13088",	
"leaf-12923",	
"leaf-12930",	
"leaf-12938",	
"leaf-12929",	
"leaf-12926",	
"leaf-13081",	
"leaf-12939",	
"leaf-12943",	
"leaf-12955",	
"leaf-12956",	
"leaf-12957",	
"leaf-12958",	
"leaf-12964",	
"leaf-12959",	
"leaf-12962",	
"leaf-12968",	
"leaf-12970",	
"leaf-12972",	
"leaf-12978",	
"leaf-12980",	
"leaf-12982",	
"leaf-12986",	
"leaf-12987",	
"leaf-13056",	
"leaf-13007",	
"leaf-13017",	
"leaf-13023",	
"leaf-13032",	
"leaf-13063",	
"leaf-13066",	
"leaf-13062",	
"leaf-13061",	
"leaf-13064",	
"leaf-13065",	
"leaf-13067",	
"leaf-13068",	
"leaf-13070",	
"leaf-13069",	
"leaf-13094",	
"leaf-13093",	
"leaf-13079",	
"leaf-13095",	
"leaf-13097",	
"leaf-13098",	
"leaf-13103",	
"leaf-13104",	
"leaf-13102",	
"leaf-13101",	
"leaf-13100",	
"leaf-13105",
"leaf-13106",	
"leaf-13107",	
"leaf-13109",	
"leaf-13111",	
"leaf-13108",	
"leaf-13119",	
"leaf-13140",	
"leaf-13142",	
"leaf-13143",	
"leaf-13144",	
"leaf-13141"};
  
  for(int64_t j=0;j<100;j++)
  {
    attackerNodes.Add(Names::Find<Node>(attacker_name[j]));   //legitimate user
  }


  

  // Install NDN applications
  std::string prefix_user = "/dst/user";
  std::string prefix_attacker= "/dst/attcker";

  //config user and attacker
  ndn::AppHelper consumerHelper("ns3::ndn::ConsumerZipfMandelbrot");
  //config user
  consumerHelper.SetAttribute("NumberOfContents", StringValue("10000"));   //total content items
  consumerHelper.SetAttribute("LifeTime", StringValue("2s"));
  consumerHelper.SetPrefix(prefix_user);
  consumerHelper.SetAttribute("Frequency", StringValue("15")); // 1000 interests a second
  consumerHelper.Install(userNodes);
  //config attacker
  consumerHelper.SetAttribute("NumberOfContents", StringValue("10000"));   //total content items
  consumerHelper.SetAttribute("LifeTime", StringValue("2s"));
  consumerHelper.SetPrefix(prefix_attacker);
  consumerHelper.SetAttribute("Frequency", StringValue("150")); // 100 interests a second
  consumerHelper.Install(attackerNodes).Start(Seconds(10));

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
  ndnGlobalRoutingHelper.AddOrigins(prefix_attacker, producer);

  // Calculate and install FIBs
  ndn::GlobalRoutingHelper::CalculateRoutes();
  
  Simulator::Stop(Seconds(20));

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

