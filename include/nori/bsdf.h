/*
    This file is part of Nori, a simple educational ray tracer

    Copyright (c) 2012 by Wenzel Jakob and Steve Marschner.

    Nori is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    Nori is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#if !defined(__BSDF_H)
#define __BSDF_H

#include <nori/object.h>

NORI_NAMESPACE_BEGIN


/**
 * \brief Convenience data structure used to pass multiple
 * parameters to the evaluation and sampling routines in \ref BSDF
 */
struct BSDFQueryRecord {
	/// Incident direction (in the local frame)
	Vector3f wi;

	/// Outgoing direction (in the local frame)
	Vector3f wo;

	/// Measure associated with the sample
	EMeasure measure;

	/// Create a new record for sampling the BSDF
	inline BSDFQueryRecord(const Vector3f &wi)
		: wi(wi), measure(EUnknownMeasure) { }

	/// Create a new record for querying the BSDF
	inline BSDFQueryRecord(const Vector3f &wi,
			const Vector3f &wo, EMeasure measure) 
		: wi(wi), wo(wo), measure(measure) { }
};

/**
 * \brief Superclass of all bidirectional scattering distribution functions
 */
class BSDF : public NoriObject {
public:
	/**
	 * \brief Sample the BSDF and return the importance weight (i.e. the
	 * value of the BSDF * cos(theta_o) divided by the probability density 
	 * of the sample with respect to solid angles). 
	 *
	 * \param bRec    A BSDF query record
	 * \param sample  A uniformly distributed sample on \f$[0,1]^2\f$
	 *
	 * \return The BSDF value divided by the probability density of the sample
	 *         sample. The returned value also includes the cosine
	 *         foreshortening factor associated with the outgoing direction,
	 *         when this is appropriate. A zero value means that sampling
	 *         failed.
	 */
	virtual Color3f sample(BSDFQueryRecord &bRec, const Point2f &sample) const = 0;

	/**
	 * \brief Evaluate the BSDF for a pair of directions and measure
	 * specified in \code bRec
	 * 
	 * \param bRec
	 *     A record with detailed information on the BSDF query
	 * \return
	 *     The BSDF value, evaluated for each color channel
	 */
	virtual Color3f eval(const BSDFQueryRecord &bRec) const = 0;

	/**
	 * \brief Compute the probability of sampling \c bRec.wo
	 * (conditioned on \c bRec.wi).
	 *
	 * This method provides access to the probability density that
	 * is realized by the \ref sample() method.
	 *
	 * \param bRec
	 *     A record with detailed information on the BSDF query
	 *
	 * \return
	 *     A probability/density value expressed with respect
	 *     to the specified measure
	 */

	virtual float pdf(const BSDFQueryRecord &bRec) const = 0;
	
	/**
	 * \brief Return the type of object (i.e. Mesh/BSDF/etc.) 
	 * provided by this instance
	 * */
	EClassType getClassType() const { return EBSDF; }
};

NORI_NAMESPACE_END

#endif /* __BSDF_H */
