/* CSD_FMIndex.h
 * Copyright (C) 2011, Rodrigo Canovas & Miguel A. Martinez-Prieto
 * all rights reserved.
 *
 * Abstract class for implementing Compressed String Dictionaries following:
 *
 *   ==========================================================================
 *     "Compressed String Dictionaries"
 *     Nieves R. Brisaboa, Rodrigo Canovas, Francisco Claude, 
 *     Miguel A. Martinez-Prieto and Gonzalo Navarro.
 *     10th Symposium on Experimental Algorithms (SEA'2011), p.136-147, 2011.
 *   ==========================================================================
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
 *   Rodrigo Canovas:  rcanovas@dcc.uchile.cl
 *   Miguel A. Martinez-Prieto:  migumar2@infor.uva.es
 */

#ifndef _CSDFMINDEX_H
#define _CSDFMINDEX_H

#include "CSD.h"

#include <SequenceBuilder.h>
#include <Sequence.h>
#include <BitSequenceBuilder.h>
#include <BitSequence.h>
#include "fmindex/SSA.h"

#include <set>
using namespace std;

namespace csd{

	class CSD_FMIndex: public CSD{	
		
		public:
			/** General constructor **/
			CSD_FMIndex();

			/** Constructor reading Tdict from a file named 'filename'. The file must
			 * contain one word per line. 
			 * @filename: path to the file containing the string dictionary.
			 * @sparse_bitsequence: tell which rank/select implementation will be use into the FMIndex
			 *                      false->BitSequeceRG and true->BitSequenceRRR
			 * @bparam:  If sparce_bitsequence==false  bparam can be (2,3,4,20,40). Otherwise it is the sample rate of BitSequenceRRR
			 * @bwt_sample: sample range that will used for the bwt
			 * @use_sample: tell if the suffixes sampling will be stored or not.
			 **/
			CSD_FMIndex(char *filename, bool sparse_bitsequence=false, int bparam=40, size_t bwt_sample=64, bool use_sample=false);

			/** Constructor receiving Tdict as a sequence of 'tlength' uchars. Tdict
			 * must concatenate all strings separated by '\n' characters.  
			 * @dict: uchar stream representing the string dictionary.
			 * @tlength: number of integer symbols in the stream.
			 * @sparse_bitsequence: tell which rank/select implementation will be use into the FMIndex
			 *                       false->BitSequeceRG and true->BitSequenceRRR
			 * @bparam:  If sparce_bitsequence==false  bparam can be (2,3,4,20,40). Otherwise it is the sample rate of BitSequenceRRR
			 * @bwt_sample: sample range that will used for the bwt
			 * @use_sample: tell if the suffixes sampling will be stored or not.
			 **/
			CSD_FMIndex(uchar *dict, uint tlength, bool sparse_bitsequence=false, int bparam=40, size_t bwt_sample=64, bool use_sample=false);
			
			/** Constructor receiving Tdict as a sequence of 'tlength' uchars. Tdict
			 * @param it: Iterator uchar
			 * @param stopword: in which prefix stop the iteration
			 * @sparse_bitsequence: tell which rank/select implementation will be use into the FMIndex
			 *                       false->BitSequeceRG and true->BitSequenceRRR
			 * @bparam:  If sparce_bitsequence==false  bparam can be (2,3,4,20,40). Otherwise it is the sample rate of BitSequenceRRR
			 * @bwt_sample: sample range that will used for the bwt
			 * @use_sample: tell if the suffixes sampling will be stored or not.
			 * @param listener
			 **/
			CSD_FMIndex(IteratorUCharString *it, uchar *stopword, bool sparse_bitsequence=false, int bparam=40, size_t bwt_sample=64, bool use_sample=false, hdt::ProgressListener *listener=NULL);

			CSD_FMIndex(set<string>::iterator dict_begin, set<string>::iterator dict_end, uint tlength, bool sparse_bitsequence=false, int bparam=40, size_t bwt_sample=64, bool use_sample=false);

			/** Returns the ID that identify s[1..length]. If it does not exist, 
			 * returns 0. 
			 * @s: the string to be located.
			 * @len: the length (in characters) of the string s.
			 * */
			uint32_t locate(const uchar *s, uint32_t len);

			/** Returns the number of IDs that contain s[1,..len] as a substring. It also 
			 * return in occs the IDs. Otherwise return 0.
			 *  @s: the substring to be located.
			 *  @len: the length (in characters) of the string s.
			 *  @occs: pointer where the ID located will be stored.
			 * */
			uint32_t locate_substring(uchar *s, uint32_t len, uint32_t **occs);

			/** Returns the string identified by id.
			 * @id: the identifier to be extracted.
			 **/
			uchar * extract(uint32_t id);

			/** Obtains the original Tdict from its CSD_RePairDAC representation. Each 
			 * string is separated by '\n' symbols.
			 * @dict: the plain uncompressed dictionary.
			 * @return: number of total symbols in the dictionary.
			 * */
			uint decompress(uchar **dict);

			void dumpAll();

			/** Returns the size of the structure in bytes. */
			uint64_t getSize();

		
			/** Stores a CSD_FMIndex structure given a file pointer.
			 * @fp: pointer to the file saving a CSD_FMIndex structure.
			 * */
			void save(ofstream & fp);

			/** Loads a CSD_FMIndex structure from a file pointer.
			 * @fp: pointer to the file storing a CSD_FMIndex structure. */
			static CSD * load(ifstream & fp);

			void fillSuggestions(const char *base, vector<string> &out, int maxResults);

			/** General destructor. */
			~CSD_FMIndex();
		
		protected:
			SSA *fm_index;
			BitSequence *separators;
			bool use_sampling;

			void build_ssa(uchar *text, size_t len, bool sparse_bitsequence, int bparam, bool use_sample, size_t bwt_sample);
			
			void quicksort(size_t *occs, size_t ini, size_t len);

			size_t get_pivot(size_t *occs, size_t ini, size_t end);
	};

};
#endif  /* _URICDFMINDEX_H */