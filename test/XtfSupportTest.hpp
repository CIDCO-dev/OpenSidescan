#ifndef XTFSUPPORTTEST_H
#define XTFSUPPORTTEST_H

#include "catch.hpp"
#include <string>
#include "../src/OpenSidescan/sidescan/sidescanimager.h"
#include "../src/OpenSidescan/sidescan/sidescanfile.h"
#include "../src/thirdParty/MBES-lib/src/datagrams/DatagramParser.hpp"
#include "../src/thirdParty/MBES-lib/src/datagrams/DatagramParserFactory.hpp"
#include "../src/thirdParty/MBES-lib/src/datagrams/xtf/XtfParser.hpp"
#include <filesystem>

TEST_CASE("XTF Support Test") {
	
	class SidescanFileReader : public DatagramEventHandler{
		public:
			
			SidescanFileReader(std::string & filename):filename(filename){

			}

			/**Destroys the SidescanDataDumper*/
			~SidescanFileReader(){
				for(auto &v: channels){
					for(auto &p: v){
						delete p;
					}
				}
			}
    
			void processSidescanData(SidescanPing * ping){
				    
				    if(channels.size() < ping->getChannelNumber()+1){
				        std::vector<SidescanPing*> u;
				        channels.push_back(u);
				    }

				    channels[ping->getChannelNumber()].push_back(ping);
				}
		    
			bool generateImages(){
				
				int count = 0;
				
			    for(unsigned int i=0;i<channels.size();i++){
			        std::stringstream ss;
			        
			        ss << filename <<  "-channel-" << i << ".jpg";
			        
			        cv::Mat img(channels[i].size(),channels[i].at(0)->getSamples().size(), CV_64F,cv::Scalar(0));
			        
			        if(channels[i].size() > 0 && channels[i].at(0)->getSamples().size() > 0){
			        	count++;
			        }
				
				} //channel loop
				
				if(count == channels.size() && count != 0){
					//std::cerr << count << "\n";
					return true;
				}
				else{
					return false;
				}
			}
		
		private:
    		std::string filename;
    
    		std::vector< std::vector<SidescanPing * > > channels;

	};
	
	
	/*
	1- A backup of the dataset can be found in the NAS : /TI/Opensidescan/XTF_Support_test_dataset
	2- SciptsPython/update_sss_test_file_list.py can also be use to update the SSS file dataset
	*/
	
	std::filesystem::path filesLocation;
	
	#ifdef linux
		
		filesLocation = "/opt/XTF_Support_test_dataset/";
		
		if (!std::filesystem::exists(filesLocation)) {
			std::filesystem::path zip{"/opt/XTF_Support_test_dataset.zip"};
			
			if (std::filesystem::exists(zip)) {
				
				// depending on distribution version, cmake >= 3.18 might not be available
				// therefore extracting zip cannot be done with cmake
				std::system("unzip /opt/XTF_Support_test_dataset.zip -d /opt/");
			}
			else{
				std::cerr<<"Zip and dataset cannot be found" << std::endl;
				REQUIRE(true == false);
			}
		}
	#endif
	
	
	#ifdef _WIN32
		
		filesLocation = "C:\\XTF_Support_test_dataset\\";
		
		if (!std::filesystem::exists(filesLocation)) {
			std::cerr<<"Dataset cannot be found" << std::endl;
			REQUIRE(true == false);
		}
		
	#endif
	
	

	
	
	
	for (auto const& dir_entry : std::filesystem::directory_iterator{filesLocation}){
		std::string fileName = dir_entry.path().string();
        if(dir_entry.path().extension().string() == ".xtf"){
			try{
		    	
		    	DatagramParser *parser = NULL;
		    	
		        SidescanFileReader sidescan(fileName);

				std::cerr << "[+] Decoding " << fileName << std::endl;
				
				parser = DatagramParserFactory::build(fileName, sidescan);

				parser->parse(fileName);
				 
				REQUIRE(sidescan.generateImages());
				
				delete parser;
			}
			catch(std::exception * e){
				std::cerr << "[-] Error while parsing " << fileName << ": " << e->what() << std::endl;
				// Without this line the test wont tell us that the test failed
				// "CHECK"" allows the other following tests to be executed
				CHECK(true == false);
			}
		}   
	}
}
#endif
