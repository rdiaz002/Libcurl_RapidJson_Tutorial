#include <curl/curl.h>
//#include <stdio.h>
#include <iostream>
#include <string>
#include<algorithm>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#define DEBUG_ON 0
//callback to handle incoming data. 

static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp){
	
	std::cout<<nmemb<<'\n';
	std::cout<<size<<'\n';
	((std::string *)userp)->append((char*)buffer,nmemb);
	
	return nmemb;
}

using namespace rapidjson;

int main(){
	CURL * curl;
	curl = curl_easy_init();
	std::string data;
	//Setting curl options for operations. 
	curl_easy_setopt(curl, CURLOPT_URL,"https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=MSFT&apikey=demo");
	curl_easy_setopt(curl,CURLOPT_WRITEDATA, &data); //where we will write our download data.
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, write_data); 
	curl_easy_setopt(curl,CURLOPT_FAILONERROR,1L); //check for errors in internet(404...)

	int result = curl_easy_perform(curl); // perform the action that is requested. 

	
	curl_easy_cleanup(curl); //clean up our setup.
	
	
	  // 1. Parse a JSON string into DOM.
    
	data.erase(std::remove(data.begin(),data.end(),'\n'),data.end());
	
	#if DEBUG_ON
	std::cout<<data<<std::endl;
	#endif


	const char* json = data.c_str();
    Document d;
    d.Parse(json);

	#if DEBUG_ON
	//Takes global quote object and outputs the name and value of each item in the object. 
	const Value & a = d["Global Quote"];

	std::cout<<a.IsObject()<<'\n';

	for(Value::ConstMemberIterator itr = a.MemberBegin();itr!=a.MemberEnd();itr++){
		std::cout<<itr->name.GetString() <<":"<< itr->value.GetString()<<'\n';
	}
	#endif
	
    // 3. Stringify the DOM
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    
    std::cout << buffer.GetString() << std::endl;

	return 0;
}
