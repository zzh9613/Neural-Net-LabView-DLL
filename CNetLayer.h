#pragma once

#include "defininitions.h"

#ifndef CNETLAYER
#define CNETLAYER

/* Abstract base class for a layer of weights.
 * This means that this class sits in between nodes and forwards input or
 * backpropagates deltas.
 */
class CNetLayer {
	public:
		// constructors and initializers
		CNetLayer(size_t _NOUT, size_t _NIN);
		CNetLayer(size_t _NOUT, size_t _NIN, actfunc_t type);
		CNetLayer(size_t _NOUT, actfunc_t type, CNetLayer& const lower);
		virtual ~CNetLayer() {}; // purely abstract

		// type
		virtual layer_t whoAmI() const;
		// forProp
		virtual void forProp(MAT& in, bool saveActivation) = 0; // recursive
		// backprop
		virtual MAT grad(MAT& const input) =0;
		virtual void backPropDelta(MAT& const delta) = 0; // recursive
		virtual fREAL applyUpdate(learnPars pars, MAT& const input) =0 ; // recursive
		void NesterovWeightSetback(learnPars pars); // recursive

		// getters
		inline size_t getNIN() const { return NIN; };
		inline size_t getNOUT() const { return NOUT; };
		MAT getDACT() const;
		MAT getACT() const;
		// Connect to layer above and change hierarchy from output to hidden
		void connectAbove(CNetLayer* ptr);
		void resetConjugate(MAT& const input);

		// save to file
		friend ostream& operator<<(ostream& os, const CNetLayer& toSave); // almost virtual member
		friend ifstream& operator >> (ifstream& in, CNetLayer& toReconstruct);

	protected:
		MAT layer; // actual layer
		MAT vel; // velocity for momentum OR previous gradient
		MAT prevStep; // this is needed for conjugate gradient method
		MAT actSave; // keep activation before propagation
		MAT deltaSave; // store deltas for backprop

		// saving functions
		void saveMother(ostream& os) const;
		void reconstructMother(istream& is) ;
		virtual void saveToFile(ostream& os) const = 0;
		virtual void loadFromFile(ifstream& in) = 0;
		// could be made private
		actfunc_t activationType; // store the type of activation function
		ACTFUNC act; // pointer to the activation function
		ACTFUNC dact; // pointer to the activation function derivative
		void assignActFunc(actfunc_t type);
		CNetLayer*  below; // store pointer to layer below ... 
		CNetLayer* above; // and above. 

		hierarchy_t hierarchy;

		size_t NOUT; // If 2D then NOUT = NOUTXY*NOUTXY
		size_t NIN; // If 2D then NIN = NINXY*NINXY. Does not contain the bias node. 

};

#endif