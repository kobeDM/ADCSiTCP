#include <string.h>
#include <fstream>

#include "TChain.h"

void chain(const std::string chainListPath) {
	const std::string header = "chain";

	std::ifstream ifs(chainListPath);
	if (!ifs) {
		std::cout << "Error: Chain list cannot open !!" << std::endl;
		return;
	}

	TChain *chain = new TChain("tree", "tree");
	std::string line;

	while(getline(ifs, line)) {
		std::cout << "Adding " << line << " ..." << endl;
		chain->Add(line.c_str());
	}

	chain->Merge(Form("%s.root",header.c_str()),"recreate");
	std::cout << "Chain file was created" << std::endl;

	return;
}
