/*************************************************************
* Name      : Brian Worts
* Class     : CSC 215-05
* When      : Spring 2018
* Professor : DeGood
* Assignment: Mini Web Browser
*
* Summary: This code will take a URL and display the HTML
*              content as a text string.
*
* to compile: g++ Project3.cpp getWebPage_win.cpp -o web.exe -lws2_32
*****************************************************************/
/*
Description: This code takes a starting URL and retrieves the html text from that URL. It then searches through 
the html text for "<a href=" which signals the start of a URL. It also is followed by many non-URLs
so once it has what is between the quotation marks that follow "<a href=", it searches through that
for characters or strings that indicate it is not a URL. If it finds a URL, it is added to a string
vector. That vector is then iterated through, couted, and the same process done on the first URL is done to
the URLs in the vector to get the 2nd depth URLs.
*/
#include <iostream>
#include <vector>
#include <string>

using namespace std;

//Function Declarations
string getHTMLText (string URL);
int getWebPage (const string url, string& HTMLString);
vector<string> combineURLs(string URL); //Function that returns a vector
string getUrl(string htmlText, size_t& startLocation, size_t& endLocation);


int main() {

	string htmlText;
	string startingURL;
	vector<string>urlVector;
	vector<string>depth2URLVector;
	string depth2HTMLText;

	startingURL = "www.msn.com";
	
    htmlText = getHTMLText(startingURL); //Run funct on starting URL to return HTML text
	urlVector= combineURLs(htmlText); //Run funt on HTML text to find URLs
	
	cout << "Searching " << startingURL << " for links..." << endl;
 
 //Displays depth 1 links, then searches those links and displays those valid links
    for (int i=1; i < urlVector.size()-1; i++){  //Start at i=1 because first value is empty
		cout << "\tURL #" << i << " Depth 1: " << urlVector.at(i) << endl;
		//DEBUG: cout << "\tURL Depth 1 Size: " << urlVector.at(i).size() << endl;
		
		depth2HTMLText = getHTMLText(urlVector.at(i)); //Runs funct to get html text from the corresponding index in urlVector
		depth2URLVector = combineURLs(depth2HTMLText); //Runs funct to search the html for URLs
		//DEBUG: cout << "depth2Vector size: " << depth2URLVector.size() << endl;
		if (depth2URLVector.size()<=1){
			cout << "\t\tNo Depth 2 URLs" << endl;
		}
		else{
			for (int j=1; j < depth2URLVector.size(); j++){ //Start at j=1 because first string is blank
				cout << "\t\tURL #" << j << " Depth 2:" << depth2URLVector.at(j) << endl;
			}
		}
	} //End for loop

	
	
    return 0;
    
} // end of main()

string getHTMLText (string URL){ //Put parameter for website, returns html as string. Will cout error with error number from getWebPage file if there is one

	string html = "";
	int result;

    result = getWebPage (URL, html); // returns 0 if successful, also changes val of html

    if (result == 0){
		//cout << "Success" << endl;  //cout << html; to make it out the code. At end get rid of this
	}
    else {
		cout << "\t\tError " << result << " fetching " << URL << endl;
	}

    return html;
}

vector<string> combineURLs(string htmlText){
	string preURL;
	size_t startLocation;
	size_t endLocation;
	string url;
	vector<string>urlList(1);  //output

	string startingURL;
	startingURL = "www.msn.com";   //In future iteration add to parameter of function so don't need to declare again
	
	
	//Find <a then from there search for href = 
	preURL =   "<a"; //Signals a url follows this
	startLocation = htmlText.find(preURL); 

	

	//Finds location of preUrl. Then finds location of first and next quotation mark.
	//Takes url as the thing between those two points.
	while (startLocation != string::npos){ //Start location will change each iteration
		if (startLocation == string :: npos){
			cout << "No URL Found";
			break;
		}
		else {
			url = getUrl(htmlText, startLocation, endLocation); //endLocation not necessary but there for debug reasons
		}
		
		if (url.size() > 2){ //Finds possible urls that do not start with http or www
			
			if (url.substr(0,2) == "//"){ 
					url.insert(0,"http:");
			}
			if ((url.at(0) == '/') && (url.at(1) != '/')){
					url.insert(0,startingURL);
			}
		}
			
		//If url contains "www" or "http", will do more searches to be sure it is a url and then add to urlList
		if ((url.find("http:") != string::npos) || (url.find("www") != string ::npos))
		{	
			
			if (url.size() <= 1){ //number here is an arbitrary choice
				continue;
			}
			else if(url.at(0) == '#'){ //Will not add these to URL List
				continue;
			}
			else if(url.substr(0,10) == "javascript" ){ //Will not add these to URL List
				continue;
			}
			else {
				urlList.push_back(url);
			}
		}
	} //End while
		/* DEBUG
		for (int i = 0; i<urlList.size() ; i++){
		cout << urlList.at(i) << endl;}
		cout << "endLocation = " << endLocation << endl;
		*/
	return urlList;
	}


string getUrl(string htmlText, size_t& startLocation, size_t& endLocation){
	
	size_t preUrlStart
	size_t urlStart;
	size_t urlEnd;
	size_t urlLength;
	string finalUrl;
	
	//Finds location of preUrl. Then finds location of first and next quotation mark.
	//Takes url as the thing between those two points.
		preUrlStart = htmlText.find ("href=",startLocation);
		urlStart = htmlText.find("\"",preUrlStart); //find(search item,search start location)
		if (urlStart == string::npos){
			cout << "no url start found" << endl;
			}
		else {
			urlEnd = htmlText.find("\"",urlStart+1);
		}
	
	urlLength = urlEnd-urlStart;
	finalUrl = htmlText.substr(urlStart+1,urlLength-1);  //subdtr(start location, string length)
	endLocation = urlEnd;
	startLocation = endLocation;
	/*DEBUG
	cout << "urlStart= " << urlStart << endl << "urlEnd" << urlEnd <<endl;
	cout<< "urlEnd= " << urlEnd << endl;
	cout << "finalURL = " << finalUrl << endl;
	*/
	
	
	return finalUrl;
}

/*Notes:

<a hreF = "url"
find starting quote after location +1, then ending quotation mark -1
use pos with find string. pos starts seacrh after specified pos
find returns  string::npos when not found

while (urlFound)    //If statement saying if

        if (URL == 0){
            urlFound = false;
        }
        else {
                cout << "Link " << linkDepth;

            //URLList.push_back(i)
        }
		
		Web pages written in html
ctrl u gives html code of a website

<h1> is for header 1
<br> is break
<p /> creates paragraph
written in any text edtor
browser runs on your computer
server hosts website. server can be computer

building a webpage
tags: html title body h1 a 

search for href but not everything after href is a webpage
if href = “#...”  ignore because is bookmark
href = “javascript…” ignore

c++ functions
size()     .at()      find()      substr()       replace()     compare()

calling a function within itself is called recursion
void proj3 (string url, int depth) { //retireve url
loop to find anchor
if (depth >0)
	print new url
	proj3(new Url, depth-1)
anchor becomes new anchor

Can also do project using vectors
add url to vector and fill vector each time

*/