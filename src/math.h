#ifndef MATH_H
#define MATH_H

#include "vertex.h"

/**
 * \brief Compute the cross-product of two vectors.
 * \param u the first vector
 * \param v the second vector
 * \param result the vector to store the result in
 */
void math_cross3f(vertex3f_t u, vertex3f_t v, vertex3f_t result);

/**
 * \brief Compute the barycentric coordinates of point p with respect to triangle abc.
 * \param a the first point of the base traingle
 * \param b the second point of the base triangle
 * \param c the third point of the base triangle
 * \param p the point of who's coordinates to calculate
 * \param result the vector to store the result in
 */
void math_barycentric2i(vertex2i_t a, vertex2i_t b, vertex2i_t c, vertex2i_t p, vertex3f_t result);
void math_barycentric3f(vertex3f_t a, vertex3f_t b, vertex3f_t c, vertex3f_t p, vertex3f_t result);

/**
 * \brief Compute the face normal of a triangle
 * \param a the first point of the traingle
 * \param b the second point of the triangle
 * \param c the third point of the triangle
 * \param result the vector to store the result in
 */
void math_normal(vertex3f_t a, vertex3f_t b, vertex3f_t c, vertex3f_t result);

/**
 * \brief Normalize a vector by dividing each component with the vector's length
 * \param v the vector to normalize
 */
void math_normalize(vertex3f_t v);

/**
 * \brief compute the dot product of two vectors
 * \param u the first vector
 * \param v the second vector
 * \return the dot product
 */
float math_dotproduct(vertex3f_t u, vertex3f_t v);

#endif /* MATH_H */
