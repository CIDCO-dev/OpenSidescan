/*
* Copyright 2019 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/


#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include "../thirdParty/MBES-lib/src/utils/StringUtils.hpp"
#include "../OpenSidescan/sidescan/sidescanimager.h"
#include "../OpenSidescan/sidescan/sidescanfile.h"
#include "../OpenSidescan/sidescan/sidescanimage.h"
//#include "../OpenSidescan/utilities/opencvhelper.h"
#include "../OpenSidescan/inventoryobject/inventoryobject.h"
#include "../OpenSidescan/detector/roidetector.h"

#include <Eigen/Dense>

#define POPULATION_SIZE 100
#define DECIMATION_SIZE 80

typedef struct{
    int channel;
    int x;
    int y;
} hit ;

typedef struct{
    int fastThreshold;
    int dbscanEpsilon;
    int dbscanMinPts;
    int mserDelta;
    int mserMinArea;
    int mserMaxArea;
    
    int fitness;
} genome;

void printUsage(){
    std::cerr << "fitDetector directory" << std::endl;
    exit(1);
}

void loadFiles(std::vector<SidescanFile*> & files,std::vector<std::vector<hit*> *> & hits, std::string & directoryPath){
    DIR *dir = NULL;
    
    //TODO: read this lever arm from metadata or user input
    Eigen::Vector3d antenna2TowPoint(0.0,0.0,0.0);
    
    if(dir = opendir(directoryPath.c_str())){
        
        std::cerr << "Processing " << directoryPath << std::endl;
        
        dirent* file;
        
        std::stringstream filenameStream;
        
        while(file=readdir(dir)){
            
            if(StringUtils::ends_with(file->d_name,".xtf")){               
                filenameStream.str("");
                filenameStream << directoryPath << "/" << file->d_name;
                std::string fileName(filenameStream.str());
                
                std::cerr << "[+] Processing " << fileName << std::endl;
                
                std::cerr << "[-]     Reading image data from " << fileName << std::endl;                
                
                //Read image data
                SidescanImager imager;
                DatagramParser * parser = DatagramParserFactory::build(fileName,imager);
                parser->parse(fileName);

                SidescanFile * sidescanFile = imager.generate(fileName, antenna2TowPoint);

                files.push_back(sidescanFile);
                
                //delete parser;
                
                //Read hits -------------------------------------                
                filenameStream.str("");
                filenameStream << directoryPath << "/" << file->d_name << ".hits";
                std::string hitsFilename = filenameStream.str();
                
                std::cerr << "[-]     Reading hits data from " << hitsFilename << std::endl;
                
                std::ifstream hitsFile(hitsFilename);
                
                if(hitsFile){
                    std::string line;
                    std::vector<hit*> * fileHits = new std::vector<hit*>();
                    
                    while(std::getline(hitsFile,line)){
                        hit * h = (hit*) malloc(sizeof(hit));
                        
                        if(sscanf(line.c_str(),"%d %d %d",&h->channel,&h->x,&h->y) == 3){
                            fileHits->push_back(h);
                        }
                    }
                    std::cerr << "[+]     " << fileHits->size() << " hits read" << std::endl;
                    hits.push_back(fileHits);
                    hitsFile.close();
                }
            }
        }
        
        closedir(dir);
    }
    else{
        std::cerr << "Can't open directory " << directoryPath << std::endl;
    }
}

void randomize(genome * g){
        g->fastThreshold= rand() % 300 + 100;
        g->dbscanEpsilon= rand() % 100 + 10;
        g->dbscanMinPts = rand() % 10 + 10;
        g->mserDelta= rand() % 10 + 5;
        g->mserMinArea= rand() % 1000 + 10;
        g->mserMaxArea = 15000;    
}

void initGenomes(std::vector<genome*> & genomes){
    std::cerr << "[+] Initializing " << POPULATION_SIZE << " genomes" << std::endl;
    
    for(int i=0;i<POPULATION_SIZE;i++){
        genome * g = (genome*)malloc(sizeof(genome));
        
        randomize(g);
        
        genomes.push_back(g);
    }
}

bool sortGenome (genome* i,genome* j) { return (i->fitness>j->fitness); }

void decimate(std::vector<genome*> & genomes){
    std::cerr << "[+] Decimating" << std::endl;
    
    std::sort(genomes.begin(),genomes.end(),sortGenome);
    
    for(auto i=genomes.begin()+(POPULATION_SIZE-DECIMATION_SIZE);i!=genomes.end();i++){
        free(*i);
    }
    
    genomes.resize(POPULATION_SIZE - DECIMATION_SIZE);
}

genome * crossover(genome * f,genome * g){
    genome * h = (genome*)malloc(sizeof(genome));
    
    h->fastThreshold= (rand() % 100 > 50)? g->fastThreshold : f->fastThreshold;
    h->dbscanEpsilon= (rand() % 100 > 50)? g->dbscanEpsilon : f->dbscanEpsilon;
    h->dbscanMinPts = (rand() % 100 > 50)? g->dbscanMinPts  : f->dbscanMinPts;
    h->mserDelta= (rand() % 100 > 50)? g->mserDelta : f->mserDelta;
    h->mserMinArea= (rand() % 100 > 50)? g->mserMinArea : f->mserMinArea;
    h->mserMaxArea= (rand() % 100 > 50)? g->mserMaxArea : f->mserMaxArea;
    h->fitness = 0;
    
    return h;
}

void mutate(genome * h){
    
    int mutation = (rand() % 20) - 10; // -10 to 10
    
    if(rand() % 100 > 90 && h->fastThreshold + mutation > 0){
        h->fastThreshold= h->fastThreshold + mutation;
    }
    
    if(rand() % 100 > 90 && h->dbscanEpsilon + mutation > 0){
        h->dbscanEpsilon= h->dbscanEpsilon  + mutation;
    }
    
    if(rand() % 100 > 90 && h->dbscanMinPts + mutation > 0){
        h->dbscanMinPts= h->dbscanMinPts + mutation;
    }
    
    if(rand() % 100 > 90 && h->mserDelta + mutation >0){
        h->mserDelta= h->mserDelta + mutation;   
    }
    
    if(rand() % 100 > 90 && h->mserMinArea + mutation > 0){
        h->mserMinArea= h->mserMinArea + mutation;
    }
    
}

void repopulate(std::vector<genome*> & genomes){
    std::vector<genome*> offsprings;
    
    std::cerr << "[+] Repopulating..." << std::endl;    
    
    while(offsprings.size()<DECIMATION_SIZE){
        genome * f = genomes.at(rand() % genomes.size());        
        genome * g = genomes.at(rand() % genomes.size());
        
        if(f != g){
            genome * offspring = crossover(f,g);
            mutate(offspring);
            offsprings.push_back(offspring);
        }
    }
    
    genomes.insert(genomes.end(),offsprings.begin(),offsprings.end());
}

void cancerize(std::vector<genome*> & genomes){
    std::vector<genome*> offsprings;
    
    std::cerr << "[+] Cancerizing..." << std::endl;    
    
    while(offsprings.size()<DECIMATION_SIZE){
        genome * g = (genome*)malloc(sizeof(genome));
        randomize(g);
        offsprings.push_back(g);
    }
    
    genomes.insert(genomes.end(),offsprings.begin(),offsprings.end());    
}

bool insideHits(InventoryObject * obj,std::vector<hit*> & hits){
    for(auto i=hits.begin();i!=hits.end();i++){
        if(
                (*i)->channel == obj->getImage().getChannelNumber() &&
                ((*i)->x >= obj->getX()) && ((*i)->x <= (obj->getX() + obj->getPixelWidth())) &&
                ((*i)->y >= obj->getY()) && ((*i)->y <= (obj->getY() + obj->getPixelHeight()))
        ){
            return true;
        }
    }
    
    return false;
}

bool insideDetections(hit * h, std::vector<InventoryObject*> & detections){
    for(auto i=detections.begin();i!=detections.end();i++){
        if(
                h->channel == (*i)->getImage().getChannelNumber() &&
                (h->x >= (*i)->getX()) && (h->x <= ((*i)->getX() + (*i)->getPixelWidth())) &&
                (h->y >= (*i)->getY()) && (h->y <= ((*i)->getY() + (*i)->getPixelHeight()))                
        ){
            return true;
        }
    }
    
    return false;
}

genome* updateFitnesses(std::vector<genome*> & genomes,std::vector<SidescanFile*> & files,std::vector<std::vector<hit*> *> & hits){
    genome* bestFit = NULL;
    
    std::cerr << "[+] Updating fitness values" << std::endl;
    
    //for every genome
    for(auto g=genomes.begin();g!=genomes.end();g++){       
        
        if(!bestFit) bestFit=*g;
        
        //recompute fitness
        double fitness = 0.0;
        
        int truePositive   = 0;
        int precisionCount = 0;
        
        int recalled = 0;
        int recallCount = 0;
        
        //using each file
        for(unsigned int fileIdx=0;fileIdx<files.size();fileIdx++){
            
            std::vector<InventoryObject*> detections;            
 
                    //and each image
            for(auto i=files[fileIdx]->getImages().begin();i!=files[fileIdx]->getImages().end();i++){                
                    //TODO : selector detector type
                    RoiDetector roiDetector(
                                            (*g)->fastThreshold,
                                            cv::FastFeatureDetector::TYPE_9_16,
                                            false,
                                            (*g)->dbscanEpsilon,
                                            (*g)->dbscanMinPts,
                                            (*g)->mserDelta,
                                            (*g)->mserMinArea,
                                            (*g)->mserMaxArea,
                                            true
                                           );                 
            }
            
            //update precision stats
            for(auto detection=detections.begin();detection != detections.end(); detection++){
                if(insideHits(*detection,* hits[fileIdx])){
                    //std::cerr << "HIT" << std::endl;
                    truePositive++;
                }
                
                precisionCount++;
            }
            
            //update recall stats
            for(auto h=hits[fileIdx]->begin(); h!=hits[fileIdx]->end(); h++){
                if(insideDetections(*h,detections)){
                    recalled++;
                }
                
                recallCount++;
            }
            
            for(auto i=detections.begin();i!=detections.end();i++){
                delete (*i);
            }
        }

        //compute fitness
        double precision = (truePositive > 0 && precisionCount > 0)?((double)truePositive/(double)precisionCount) * 100 : 0.0;
        double recall = (recalled > 0 && recallCount > 0)?((double)recalled/(double)recallCount)*100:0.0; //TODO: add recall
        
        std::cerr << "[-] " << precision << "% / " << recall << " % (" << (*g)->fastThreshold << " " << (*g)->dbscanEpsilon << " " << (*g)->dbscanMinPts << " " << (*g)->mserDelta << " " << (*g)->mserMinArea << " " << (*g)->mserMaxArea << " )" << std::endl;
        
        fitness = precision + recall; 
        
        (*g)->fitness = fitness;
        
        if(bestFit->fitness < fitness)  bestFit= (*g);
    }
    
    return bestFit;
}


int main(int argc,char** argv){
    
    if(argc != 2){
        printUsage();
        return 1;
    }
    
    srand (time(NULL));    
    
    std::string directory = argv[1];
    
    std::vector<SidescanFile*>       files;
    std::vector<std::vector<hit*> *> hits;

    std::vector<genome*>             genomes;
    
    double lastFit = 0.0;
    
    genome * bestFit= NULL;    
    double fitThreshold   = 199.999;

    int nbGen = 0;    
    int genMaxCount = 1000;
    
    
    try{
        initGenomes(genomes);
        loadFiles(files,hits,directory);
        
        while((!bestFit || bestFit->fitness < fitThreshold) && nbGen < genMaxCount){
            std::cerr << "[+] Generation " << nbGen << std::endl;
            
            bestFit = updateFitnesses(genomes,files,hits);
            
            std::cerr << "[-] Best fitness: " << bestFit->fitness << std::endl;
            
            if(bestFit->fitness > fitThreshold){
                break;
            }
            
            //compute progress
            double progress = bestFit->fitness - lastFit;
            
            if(progress > 0){
                decimate(genomes);
                repopulate(genomes);        
            }
            else{
                decimate(genomes);
                cancerize(genomes);
            }
            nbGen++;
            lastFit = bestFit->fitness;
        }
        
        std::cerr << "[-] " << bestFit->fitness << "%  " << "(" << bestFit->fastThreshold << " " << bestFit->dbscanEpsilon << " " << bestFit->dbscanMinPts << " " << bestFit->mserDelta << " " << bestFit->mserMinArea << " " << bestFit->mserMaxArea << " )" << std::endl;
        
    }
    catch(Exception * e){
        std::cerr << "Error: " << e->what() << std::endl;
    }
    
    return 0;
}
