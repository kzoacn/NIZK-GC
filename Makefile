all:
	g++  prover.cpp -o prover -O2 -l crypto
	g++  verifier.cpp -o verifier -O2 -l crypto


