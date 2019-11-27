/*************************************************************
* Name      : DeGood
* Class     : CSC 215-05
* When      : Spring 2018
* Professor : DeGood
* 
* Assignment: Mini Web Browser
*
* Description: This example will take a URL and display the HTML
*              content as a text string.
* 
*			   The corresponding getWebPage_xxx.cpp file needs
*              to be located in the same directory as this file.
*
*              Below are how to compile this on Mac or Windows:
*
* Mac     : g++ browser.cpp getWebPage_mac.cpp -o web
* Windows : g++ browser.cpp getWebPage_win.cpp -o web.exe -lws2_32
**************************************************************/
#include <iostream>

using namespace std;

// The getWebPage() function takes a URL (Uniform Resource
// Locator) and returns the web page HTML as a C++ string. The
// return value will be zero if everything was successful or
// non-zero if some error occurred.
int getWebPage (const string url, string& HTMLString);

int main() {
	
	const string URL = "www.msn.com";
	string html = "";
	int result;
	
	/*************************************************************
	* Fetch the web page at variable URL
	* The page will be returned in variable html
	*************************************************************/
	result = getWebPage (URL, html);
	
	// Display the web page as text if everything went well.
	if (result == 0) {
		cout << html;
	// If there was an error, display it.
	} else {
		cout << "Error " << result << " fetching " << URL << endl;
	}
	
} // end of main()
