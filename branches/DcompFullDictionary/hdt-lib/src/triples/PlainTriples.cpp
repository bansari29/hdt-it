/*
 * File: PlainTriples.cpp
 * Last modified: $Date$
 * Revision: $Revision$
 * Last modified by: $Author$
 *
 * Copyright (C) 2012, Mario Arias, Javier D. Fernandez, Miguel A. Martinez-Prieto
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *
 * Contacting the authors:
 *   Mario Arias:               mario.arias@gmail.com
 *   Javier D. Fernandez:       jfergar@infor.uva.es
 *   Miguel A. Martinez-Prieto: migumar2@infor.uva.es
 *
 */

#include <HDTVocabulary.hpp>

#include "PlainTriples.hpp"
#include "TripleOrderConvert.hpp"

namespace hdt {

PlainTriples::PlainTriples() : order(SPO) {
	streamX = IntSequence::getArray(spec.get("stream.x"));
	streamY = IntSequence::getArray(spec.get("stream.y"));
	streamZ = IntSequence::getArray(spec.get("stream.z"));
}

PlainTriples::PlainTriples(HDTSpecification &specification) : spec(specification) {
	std::string orderStr = spec.get("triples.component.order");
	order = parseOrder(orderStr.c_str());
	if(order==Unknown) {
			order = SPO;
	}
	streamX = IntSequence::getArray(spec.get("stream.x"));
	streamY = IntSequence::getArray(spec.get("stream.y"));
	streamZ = IntSequence::getArray(spec.get("stream.z"));
}

PlainTriples::~PlainTriples() {
	delete streamX;
	delete streamY;
	delete streamZ;
}

float PlainTriples::cost(TripleID & triple)
{
	return 0;
}

void PlainTriples::load(ModifiableTriples &triples, ProgressListener *listener) {
	triples.sort(order);

	IntermediateListener iListener(listener);

	iListener.setRange(0,33);
	iListener.notifyProgress(0, "PlainTriples Importing subjects");
	IteratorTripleID *itS = triples.searchAll();
	ComponentIterator subjIt(itS, SUBJECT);
	streamX->add(subjIt);
	delete itS;

	iListener.setRange(33, 66);
	iListener.notifyProgress(0, "PlainTriples Importing predicates");
	IteratorTripleID *itP = triples.searchAll();
	ComponentIterator predIt(itS, PREDICATE);
	streamY->add(predIt);
	delete itP;

	iListener.setRange(66, 100);
	iListener.notifyProgress(0, "PlainTriples Importing objects");
	IteratorTripleID *itO = triples.searchAll();
	ComponentIterator objIt(itS, OBJECT);
	streamZ->add(objIt);
	delete itO;
}

void PlainTriples::populateHeader(Header &header, string rootNode) {
	header.insert(rootNode, HDTVocabulary::TRIPLES_TYPE, HDTVocabulary::TRIPLES_TYPE_PLAIN);
	header.insert(rootNode, HDTVocabulary::TRIPLES_NUM_TRIPLES, getNumberOfElements() );
	header.insert(rootNode, HDTVocabulary::TRIPLES_ORDER, getOrderStr(order) );
	header.insert(rootNode, HDTVocabulary::TRIPLES_STREAMX_TYPE, streamX->getType() );
	header.insert(rootNode, HDTVocabulary::TRIPLES_STREAMY_TYPE, streamY->getType() );
	header.insert(rootNode, HDTVocabulary::TRIPLES_STREAMZ_TYPE, streamZ->getType() );
}

IteratorTripleID *PlainTriples::search(TripleID & pattern)
{
	if(pattern.isEmpty()) {
		return new PlainTriplesIterator(this, pattern, order);
	} else {
		return new SequentialSearchIteratorTripleID(pattern, new PlainTriplesIterator(this,pattern, order));
	}
}

IteratorTripleID *PlainTriples::searchJoin(TripleID &a, TripleID &b, unsigned short conditions) {
	throw "Not implemented";
}

void PlainTriples::save(std::ostream & output, ControlInformation &controlInformation, ProgressListener *listener)
{
	controlInformation.clear();
	controlInformation.setUint("numTriples", getNumberOfElements());
	controlInformation.set("codification", HDTVocabulary::TRIPLES_TYPE_PLAIN);
	controlInformation.setUint("triples.component.order", order);
	controlInformation.set("stream.x", streamX->getType());
	controlInformation.set("stream.y", streamY->getType());
	controlInformation.set("stream.z", streamZ->getType());
	controlInformation.save(output);

	IntermediateListener iListener(listener);

	iListener.setRange(0,33);
	iListener.notifyProgress(0, "PlainTriples saving subjects");
	streamX->save(output);

	iListener.setRange(33, 66);
	iListener.notifyProgress(0, "PlainTriples saving predicates");
	streamY->save(output);

	iListener.setRange(66, 100);
	iListener.notifyProgress(0, "PlainTriples saving objects");
	streamZ->save(output);
}

void PlainTriples::load(std::istream &input, ControlInformation &controlInformation, ProgressListener *listener)
{
	unsigned int numTriples = controlInformation.getUint("numTriples");
	order = (TripleComponentOrder) controlInformation.getUint("triples.component.order");

	std::string typeSubjects = controlInformation.get("stream.x");
	std::string typePredicates = controlInformation.get("stream.y");
	std::string typeObjects = controlInformation.get("stream.z");

	delete streamX;
	delete streamY;
	delete streamZ;

	streamX = IntSequence::getArray(typeSubjects);
	streamY = IntSequence::getArray(typePredicates);
	streamZ = IntSequence::getArray(typeObjects);

	IntermediateListener iListener(listener);

	iListener.setRange(0,33);
	iListener.notifyProgress(0, "PlainTriples loading subjects");
	streamX->load(input);

	iListener.setRange(33, 66);
	iListener.notifyProgress(0, "PlainTriples loading predicates");
	streamY->load(input);

	iListener.setRange(66, 100);
	iListener.notifyProgress(0, "PlainTriples loading objects");
    streamZ->load(input);
}

size_t PlainTriples::load(unsigned char *ptr, unsigned char *ptrMax, ProgressListener *listener)
{
    throw "Not implemented";
}

void PlainTriples::generateIndex(ProgressListener *listener) {

}

void PlainTriples::saveIndex(std::ostream &output, ControlInformation &controlInformation, ProgressListener *listener) {

}

void PlainTriples::loadIndex(std::istream &input, ControlInformation &controlInformation, ProgressListener *listener) {

}

size_t PlainTriples::loadIndex(unsigned char *ptr, unsigned char *ptrMax, ProgressListener *listener)
{
}



unsigned int PlainTriples::getNumberOfElements()
{
	return streamX->getNumberOfElements();
}

size_t PlainTriples::size()
{
	return streamX->size()+streamY->size()+streamZ->size();
}

string PlainTriples::getType() {
    return HDTVocabulary::TRIPLES_TYPE_PLAIN;
}

TripleComponentOrder PlainTriples::getOrder()
{
    return order;
}

PlainTriplesIterator::PlainTriplesIterator(PlainTriples *triples, TripleID & pattern, TripleComponentOrder order) :
		pattern(pattern), order(order), triples(triples), pos(0)
{
}

void PlainTriplesIterator::updateOutput()
{
	returnTriple.setAll(triples->streamX->get(pos), triples->streamY->get(pos), triples->streamZ->get(pos));
}

bool PlainTriplesIterator::hasNext()
{
	return pos<triples->getNumberOfElements();
}

TripleID *PlainTriplesIterator::next()
{
	updateOutput();
	pos++;
	return &returnTriple;
}

bool PlainTriplesIterator::hasPrevious()
{
	return pos>0;
}

TripleID *PlainTriplesIterator::previous()
{
	pos--;
	updateOutput();
	return &returnTriple;
}

void PlainTriplesIterator::goToStart()
{
	pos=0;
}





ComponentIterator::ComponentIterator(IteratorTripleID *iterator, TripleComponentRole component) : role(component), it(iterator)
{
}

unsigned int ComponentIterator::next()
{
	TripleID *triple = it->next();

	switch(role){
	case SUBJECT:
		return triple->getSubject();
	case PREDICATE:
		return triple->getPredicate();
	case OBJECT:
		return triple->getObject();
	}
}

bool ComponentIterator::hasNext()
{
	return it->hasNext();
}

void ComponentIterator::goToStart()
{
	it->goToStart();
}

}


