#ifndef MATH_H
#define MATH_H

#include "vector.h"

/**
 * \brief Compute the cross-product of two vectors.
 * \param u the first vector
 * \param v the second vector
 * \param result the vector to store the result in
 */
void math_cross3f(vec3f_t u, vec3f_t v, vec3f_t result);

/**
 * \brief Compute the barycentric coordinates of point p with respect to triangle abc.
 * \param a the first point of the base traingle
 * \param b the second point of the base triangle
 * \param c the third point of the base triangle
 * \param p the point of who's coordinates to calculate
 * \param result the vector to store the result in
 */
void math_barycentric3f(vec3f_t a, vec3f_t b, vec3f_t c, vec3f_t p, vec3f_t result);

/**
 * \brief Denormalized version of the barycentric coordinates.
 * Note that the result is now stored in a vec4f_t: [a0, a1, a2, a3],
 * where components a0 to a2 have to be divided by a3 to obtain the real coordinates.
 * \param a the first point of the base traingle
 * \param b the second point of the base triangle
 * \param c the third point of the base triangle
 * \param p the point of who's coordinates to calculate
 * \param result the vector to store the result in
 */
void math_barycentric3f_denorm(vec3f_t a, vec3f_t b, vec3f_t c, vec3f_t p, vec4f_t result);

/**
 * \brief Compute the face normal of a triangle
 * \param a the first point of the traingle
 * \param b the second point of the triangle
 * \param c the third point of the triangle
 * \param result the vector to store the result in
 */
void math_normal(vec3f_t a, vec3f_t b, vec3f_t c, vec3f_t result);

/**
 * \brief Normalize a vector by dividing each component with the vector's length
 * \param v the vector to normalize
 */
void math_normalize(vec3f_t v);

/**
 * \brief compute the dot product of two vectors
 * \param u the first vector
 * \param v the second vector
 * \return the dot product
 */
float math_dotproduct(vec3f_t u, vec3f_t v);

#endif /* MATH_H */
