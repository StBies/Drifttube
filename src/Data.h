/*
 * data.h
 *
 *  Created on: Jun 12, 2017
 *      Author: bieschke
 */

#ifndef DATA_H_
#define DATA_H_

#include <array>
#include <memory>
#include <cstdlib>

//TODO delete - most probably not needed anymore

//TODO update comment
/**
 * A class for basic data types. This is basically a wrapper for arrays of 800 uint16_t values. The Data class is a base class for Events and drift time spectra.
 *
 * @brief Data class, wrapper for arrays
 *
 * @author Stefan Bieschke
 * @date June 12, 2017
 * @version Alpha 2.0
 *
 * @warning This is not meant for instanciation.
 */
template<typename T = uint16_t>
class Data
{
public:
	virtual ~Data();

	const std::array<T,800>& getData() const;
	T& operator[](const unsigned short bin) const;

protected:
	//TODO check, if constructor should be private instead of protected
	Data(std::unique_ptr<std::array<T,800>> data); //not meant for instanciation
	Data(const Data& data);
	std::unique_ptr<std::array<T,800>> m_data;
};

#endif /* DATA_H_ */
