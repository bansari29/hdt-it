/*
 * CompactTriples.hpp
 *
 *  Created on: 11/05/2011
 *      Author: mck
 */

#ifndef COMPACTTRIPLES_HPP_
#define COMPACTTRIPLES_HPP_

#include <Triples.hpp>
#include <HDTSpecification.hpp>

#include "TripleIterators.hpp"

#include "../stream/UintStream.hpp"

namespace hdt {

class CompactTriples : public Triples  {

private:
	HDTSpecification spec;
	StreamElements *streamY, *streamZ;
	unsigned int numTriples;
	TripleComponentOrder order;

public:
	CompactTriples();
	CompactTriples(HDTSpecification &specification);
	virtual ~CompactTriples();

	/**
	 * Returns a vector of triples matching the pattern
	 *
	 * @param triple
	 * @return
	 */
	IteratorTripleID *search(TripleID &triple);

	/**
	 * Calculates the cost to retrieve a specific pattern
	 *
	 * @param triple
	 * @return
	 */
	float cost(TripleID &triple);

	/**
	 * Returns the number of triples
	 *
	 * @return
	 */
	unsigned int getNumberOfElements();

	unsigned int size();

	/**
	 * Saves the triples
	 *
	 * @param output
	 * @return
	 */
	bool save(std::ostream &output, ControlInformation &controlInformation, ProgressListener *listener = NULL);

	/**
	 * Loads triples from a file
	 *
	 * @param input
	 * @return
	 */
	void load(std::istream &input, ControlInformation &controlInformation, ProgressListener *listener = NULL);

	void load(ModifiableTriples &triples, ProgressListener *listener = NULL);

	void populateHeader(Header &header, string rootNode);

	string getType();

	friend class CompactTriplesIterator;
};

class CompactTriplesIterator : public PreFetchIteratorTripleID {
private:
	CompactTriples *triples;
	unsigned int pos;
	unsigned int numTriple, masterPos, slavePos;
	unsigned int x, y, z;

public:
	CompactTriplesIterator(CompactTriples *pt, TripleID &pat);

	void getNextTriple();
};

}


#endif /* COMPACTTRIPLES_HPP_ */
