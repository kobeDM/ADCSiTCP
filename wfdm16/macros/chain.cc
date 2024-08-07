#include "inc/shinclude.h"

void chain( const std::string chainListPath, const std::string outputFilePath ) {
	
	std::ifstream ifs( chainListPath );
	if (!ifs) {
		std::cout << "Error: Chain list cannot open !!" << std::endl;
		return;
	}

	TChain *chain = new TChain( "tree", "tree" );
	std::string line;

	while(getline(ifs, line)) {
		std::cout << "Adding " << line << " ..." << endl;
		chain->Add(line.c_str());
	}

	chain->Merge( outputFilePath.c_str(), "recreate" );
	std::cout << "Chain file was created successfully." << std::endl;

	return;
}
