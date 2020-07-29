#define USE_COLUMN_MAJOR

#include <poly/Math/Vector2.h>
#include <poly/Math/Vector3.h>
#include <poly/Math/Vector4.h>
#include <poly/Math/Matrix2.h>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#define FLT_CMP(a, b) (abs(a - b) < 0.00001f ? 0 : (a < b ? -1 : 1))

using namespace poly;

///////////////////////////////////////////////////////////

TEST_CASE("Vector2", "[Vector2]")
{
	SECTION("Construct")
	{
		Vector2i v;
		REQUIRE(v.x == 0);
		REQUIRE(v.y == 0);

		v = Vector2i(10);
		REQUIRE(v.x == 10);
		REQUIRE(v.y == 10);

		v = Vector2i(3, 1);
		REQUIRE(v.x == 3);
		REQUIRE(v.y == 1);

		v = Vector2i(3.5, 5.5);
		REQUIRE(v.x == 3);
		REQUIRE(v.y == 5);
	}

	SECTION("Comparison")
	{
		Vector2i a(1, 3);
		Vector2i b(1, 3);

		REQUIRE(a == b);
		REQUIRE(!(a != b));

		a.x = 9;

		REQUIRE(!(a == b));
		REQUIRE(a != b);
	}

	SECTION("Math")
	{
		Vector2i a(3, 1);
		Vector2i b(4, 1);

		Vector2i r = a + b;
		REQUIRE(r.x == 7);
		REQUIRE(r.y == 2);

		r = a - b;
		REQUIRE(r.x == -1);
		REQUIRE(r.y == 0);

		r = a * b;
		REQUIRE(r.x == 12);
		REQUIRE(r.y == 1);

		r = a / b;
		REQUIRE(r.x == 0);
		REQUIRE(r.y == 1);

		r = a % b;
		REQUIRE(r.x == 3);
		REQUIRE(r.y == 0);
	}

	SECTION("Math")
	{
		Vector2f a(3, 1);
		Vector2f b(4, 1);

		float d = dist(a, b);
		REQUIRE(FLT_CMP(d, 1.0f) == 0);

		float l = length(a);
		REQUIRE(FLT_CMP(l, sqrt(10.0f)) == 0);

		l = length(b);
		REQUIRE(FLT_CMP(l, sqrt(17.0f)) == 0);

		d = dot(a, b);
		REQUIRE(FLT_CMP(d, 13.0f) == 0);

		l = length(normalize(a));
		REQUIRE(FLT_CMP(l, 1.0f) == 0);
	}
}

///////////////////////////////////////////////////////////

TEST_CASE("Vector3", "[Vector3]")
{
	SECTION("Construct")
	{
		Vector3i v;
		REQUIRE(v.x == 0);
		REQUIRE(v.y == 0);
		REQUIRE(v.z == 0);

		v = Vector3i(10);
		REQUIRE(v.x == 10);
		REQUIRE(v.y == 10);
		REQUIRE(v.z == 10);

		v = Vector3i(3, 1, 4);
		REQUIRE(v.x == 3);
		REQUIRE(v.y == 1);
		REQUIRE(v.z == 4);

		v = Vector3i(3.5, 5.5, 9.5);
		REQUIRE(v.x == 3);
		REQUIRE(v.y == 5);
		REQUIRE(v.z == 9);
	}

	SECTION("Comparison")
	{
		Vector3i a(1, 3, 5);
		Vector3i b(1, 3, 5);

		REQUIRE(a == b);
		REQUIRE(!(a != b));

		a.x = 9;

		REQUIRE(!(a == b));
		REQUIRE(a != b);
	}

	SECTION("Math")
	{
		Vector3i a(3, 1, 4);
		Vector3i b(1, 5, 9);

		Vector3i r = a + b;
		REQUIRE(r.x == 4);
		REQUIRE(r.y == 6);
		REQUIRE(r.z == 13);

		r = a - b;
		REQUIRE(r.x == 2);
		REQUIRE(r.y == -4);
		REQUIRE(r.z == -5);

		r = a * b;
		REQUIRE(r.x == 3);
		REQUIRE(r.y == 5);
		REQUIRE(r.z == 36);

		r = a / b;
		REQUIRE(r.x == 3);
		REQUIRE(r.y == 0);
		REQUIRE(r.z == 0);

		r = a % b;
		REQUIRE(r.x == 0);
		REQUIRE(r.y == 1);
		REQUIRE(r.z == 4);
	}

	SECTION("Math")
	{
		Vector3f a(3, 1, 4);
		Vector3f b(1, 5, 9);

		float d = dist(a, b);
		REQUIRE(FLT_CMP(d, sqrt(45.0f)) == 0);

		float l = length(a);
		REQUIRE(FLT_CMP(l, sqrt(26.0f)) == 0);

		l = length(b);
		REQUIRE(FLT_CMP(l, sqrt(107.0f)) == 0);

		d = dot(a, b);
		REQUIRE(FLT_CMP(d, 44.0f) == 0);

		l = length(normalize(a));
		REQUIRE(FLT_CMP(l, 1.0f) == 0);
	}
}

///////////////////////////////////////////////////////////

TEST_CASE("Vector4", "[Vector4]")
{
	SECTION("Construct")
	{
		Vector4i v;
		REQUIRE(v.x == 0);
		REQUIRE(v.y == 0);
		REQUIRE(v.z == 0);
		REQUIRE(v.w == 0);

		v = Vector4i(10);
		REQUIRE(v.x == 10);
		REQUIRE(v.y == 10);
		REQUIRE(v.z == 10);
		REQUIRE(v.w == 10);

		v = Vector4i(3, 1, 4, 1);
		REQUIRE(v.x == 3);
		REQUIRE(v.y == 1);
		REQUIRE(v.z == 4);
		REQUIRE(v.w == 1);

		v = Vector4i(3.5, 5.5, 9.5, 15.9);
		REQUIRE(v.x == 3);
		REQUIRE(v.y == 5);
		REQUIRE(v.z == 9);
		REQUIRE(v.w == 15);
	}

	SECTION("Comparison")
	{
		Vector4i a(1, 3, 5, 7);
		Vector4i b(1, 3, 5, 7);

		REQUIRE(a == b);
		REQUIRE(!(a != b));

		a.x = 9;

		REQUIRE(!(a == b));
		REQUIRE(a != b);
	}

	SECTION("Math")
	{
		Vector4i a(3, 1, 4, 1);
		Vector4i b(5, 9, 2, 6);

		Vector4i r = a + b;
		REQUIRE(r.x == 8);
		REQUIRE(r.y == 10);
		REQUIRE(r.z == 6);
		REQUIRE(r.w == 7);

		r = a - b;
		REQUIRE(r.x == -2);
		REQUIRE(r.y == -8);
		REQUIRE(r.z == 2);
		REQUIRE(r.w == -5);

		r = a * b;
		REQUIRE(r.x == 15);
		REQUIRE(r.y == 9);
		REQUIRE(r.z == 8);
		REQUIRE(r.w == 6);

		r = a / b;
		REQUIRE(r.x == 0);
		REQUIRE(r.y == 0);
		REQUIRE(r.z == 2);
		REQUIRE(r.w == 0);

		r = a % b;
		REQUIRE(r.x == 3);
		REQUIRE(r.y == 1);
		REQUIRE(r.z == 0);
		REQUIRE(r.w == 1);
	}

	SECTION("Math")
	{
		Vector4f a(3, 1, 4, 1);
		Vector4f b(5, 9, 2, 6);

		float d = dist(a, b);
		REQUIRE(FLT_CMP(d, sqrt(97.0f)) == 0);

		float l = length(a);
		REQUIRE(FLT_CMP(l, sqrt(27.0f)) == 0);

		l = length(b);
		REQUIRE(FLT_CMP(l, sqrt(146.0f)) == 0);

		d = dot(a, b);
		REQUIRE(FLT_CMP(d, 38.0f) == 0);

		l = length(normalize(a));
		REQUIRE(FLT_CMP(l, 1.0f) == 0);
	}
}

///////////////////////////////////////////////////////////

TEST_CASE("Matrix2", "[Matrix2]")
{
	SECTION("Construct")
	{
		Matrix2i m;
		REQUIRE(m.x.x == 1);
		REQUIRE(m.x.y == 0);
		REQUIRE(m.y.x == 0);
		REQUIRE(m.y.y == 1);

		m = Matrix2i(10);
		REQUIRE(m.x.x == 10);
		REQUIRE(m.x.y == 0);
		REQUIRE(m.y.x == 0);
		REQUIRE(m.y.y == 10);

		m = Matrix2i(3.5);
		REQUIRE(m.x.x == 3);
		REQUIRE(m.x.y == 0);
		REQUIRE(m.y.x == 0);
		REQUIRE(m.y.y == 3);

		m = Matrix2i(1, 2, 3, 4);
		REQUIRE(m.x.x == 1);
		REQUIRE(m.x.y == 2);
		REQUIRE(m.y.x == 3);
		REQUIRE(m.y.y == 4);

		m = Matrix2i(Vector2f(1.5f), Vector2f(2.5f));
		REQUIRE(m.x.x == 1);
		REQUIRE(m.x.y == 1);
		REQUIRE(m.y.x == 2);
		REQUIRE(m.y.y == 2);
	}

	SECTION("Comparison")
	{
		Matrix2i a(1, 3, 5, 7);
		Matrix2i b(1, 3, 5, 7);

		REQUIRE(a == b);
		REQUIRE(!(a != b));

		a.x.x = 9;

		REQUIRE(!(a == b));
		REQUIRE(a != b);
	}

	SECTION("Math")
	{
		Matrix2i a(3, 1, 4, 1);
		Matrix2i b(5, 9, 2, 6);

		Matrix2i r = a + b;
		REQUIRE(r.x.x == 8);
		REQUIRE(r.x.y == 10);
		REQUIRE(r.y.x == 6);
		REQUIRE(r.y.y == 7);

		r = a - b;
		REQUIRE(r.x.x == -2);
		REQUIRE(r.x.y == -8);
		REQUIRE(r.y.x == 2);
		REQUIRE(r.y.y == -5);

		r = a * b;
#ifdef USE_COLUMN_MAJOR
		REQUIRE(r.x.x == 51);
		REQUIRE(r.x.y == 14);
		REQUIRE(r.y.x == 30);
		REQUIRE(r.y.y == 8);
#else
		REQUIRE(r.x.x == 17);
		REQUIRE(r.x.y == 33);
		REQUIRE(r.y.x == 22);
		REQUIRE(r.y.y == 42);
#endif

		Vector2i v = a * Vector2i(3, 5);
#ifdef USE_COLUMN_MAJOR
		REQUIRE(v.x == 29);
		REQUIRE(v.y == 8);
#else
		REQUIRE(v.x == 14);
		REQUIRE(v.y == 17);
#endif

		r = a / b;
		REQUIRE(r.x.x == 0);
		REQUIRE(r.x.y == 0);
		REQUIRE(r.y.x == 2);
		REQUIRE(r.y.y == 0);
	}

	SECTION("Functions")
	{
		Matrix2f a(3, 1, 4, 1);

		Matrix2f r = transpose(a);
		REQUIRE(FLT_CMP(r.x.x, 3.0f) == 0);
		REQUIRE(FLT_CMP(r.x.y, 4.0f) == 0);
		REQUIRE(FLT_CMP(r.y.x, 1.0f) == 0);
		REQUIRE(FLT_CMP(r.y.y, 1.0f) == 0);

		float d = determinant(a);
		REQUIRE(FLT_CMP(d, -1.0f) == 0);

		r = a * inverse(a);
		REQUIRE(FLT_CMP(r.x.x, 1.0f) == 0);
		REQUIRE(FLT_CMP(r.x.y, 0.0f) == 0);
		REQUIRE(FLT_CMP(r.y.x, 0.0f) == 0);
		REQUIRE(FLT_CMP(r.y.y, 1.0f) == 0);

		r = inverse(a) * a;
		REQUIRE(FLT_CMP(r.x.x, 1.0f) == 0);
		REQUIRE(FLT_CMP(r.x.y, 0.0f) == 0);
		REQUIRE(FLT_CMP(r.y.x, 0.0f) == 0);
		REQUIRE(FLT_CMP(r.y.y, 1.0f) == 0);
	}
}

///////////////////////////////////////////////////////////