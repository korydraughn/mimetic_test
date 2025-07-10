#include <iostream>
#include <sstream>
#include <string>
#include <iterator>

#include <mimetic/mimetic.h>

using namespace std;
using namespace mimetic;

void printMimeStructure(MimeEntity* pMe, bool print_entity = false, int tabcount = 0)
{
	Header& h = pMe->header();                   // get header object

	for(int c = tabcount; c > 0; --c) cout << "    ";
	cout << "Content-Type: " << h.contentType() << '\n';
	cout << "Content-Type - is multipart: " << h.contentType().isMultipart() << '\n';
	cout << "Content-Type - type: " << h.contentType().type() << '\n';
	cout << "Content-Type - subtype: " << h.contentType().subtype() << '\n';

	for(int c = tabcount; c > 0; --c) cout << "    ";
	cout << "Content-Disposition: " << h.contentDisposition() << '\n';
	for(int c = tabcount; c > 0; --c) cout << "    ";
	cout << "Content-Disposition - param: name=" << h.contentDisposition().param("name") << '\n';

        // These are mostly remnants from the early days of MIME and largely serve
        // as placeholders or information for clients that might not fully understand
        // MIME formatting.
	//for(int c = tabcount; c > 0; --c) cout << "    ";
	//cout << "Preamble: " << pMe->body().preamble() << '\n';
	//for(int c = tabcount; c > 0; --c) cout << "    ";
	//cout << "Epilogue: " << pMe->body().epilogue() << '\n';

        if (print_entity) {
            for(int c = tabcount; c > 0; --c) cout << "    ";
            cout << "Body: " << pMe->body() << '\n';
        }

        cout << "**********\n";

	MimeEntityList& parts = pMe->body().parts(); // list of sub entities obj
	// cycle on sub entities list and print info of every item
	MimeEntityList::iterator mbit = parts.begin(), meit = parts.end();
	for(; mbit != meit; ++mbit)
		printMimeStructure(*mbit, true, 1 + tabcount);
}

int main()
{
	ios_base::sync_with_stdio(false);        // optimization
        istringstream ss{
//"POST / HTTP/1.1\r\n"
"Host: localhost:10000\r\n"
"User-Agent: curl/7.68.0\r\n"
"Accept: */*\r\n"
"Content-Length: 545\r\n"
"Content-Type: multipart/form-data; boundary=------------------------65fd63b92908dbad\r\n"
"\r\n"
"--------------------------65fd63b92908dbad\r\n"
"Content-Disposition: form-data; name=\"bytes\"\r\n"
"Content-Type: application/octect-stream\r\n"
"\r\n"
"KORY\r\n"
"--------------------------65fd63b92908dbad\r\n"
"Content-Disposition: form-data; name=\"key1\"\r\n"
"\r\n"
"value1\r\n"
"--------------------------65fd63b92908dbad\r\n"
// These lines are commented out to see how the library handles incomplete messages.
// The library works fine with these lines uncommented.
//"Content-Disposition: form-data; name=\"key2\"\r\n"
//"Content-Type: application/octet-stream\r\n"
//"\r\n"
//"newline\n"
//"\n"
//"\n"
//"\r\n"
//"--------------------------65fd63b92908dbad\r\n"
//"Content-Disposition: form-data; name=\"key3\"\r\n"
//"\r\n"
//"\r\n"
//"--------------------------65fd63b92908dbad\r\n"
//"Content-Disposition: form-data; name=\"key4\"\r\n"
//"\r\n"
//"\r\n"
//"--------------------------65fd63b92908dbad--\r\n"
        };
	istreambuf_iterator<char> bit(ss), eit; // get stdin iterators
	MimeEntity me(bit,eit);                       // parse and load message
	printMimeStructure(&me);                      // print msg structure

        cout << "=================================\n";

        // This is another way to setup parsing of MIME data.
        MimeEntity me2;
        me2.header().contentType("multipart/form-data; boundary=------------------------65fd63b92908dbad");
        //me2.header().push_back(Field("X-my-header", "some value"));
        const std::string data = 
            "--------------------------65fd63b92908dbad\r\n"
            "Content-Disposition: form-data; name=\"bytes\"\r\n"
            "Content-Type: application/octect-stream\r\n"
            "\r\n"
            "KORY\r\n"
            "--------------------------65fd63b92908dbad\r\n"
            "Content-Disposition: form-data; name=\"key1\"\r\n"
            "\r\n"
            "value1\r\n"
            "--------------------------65fd63b92908dbad\r\n";
        me2.load(std::begin(data), std::end(data));
	printMimeStructure(&me2);                      // print msg structure

	return 0;
}
