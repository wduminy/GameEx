#include <test_util.h>
#include <systemex.h>
#include "drome.h"
#define TESTDATA ShooterData
#define TEST(N) template<> template<> void test_group<tut::TESTDATA>::object::test<N>()
#define BEGIN(Num,Name) template<> template<> void test_group<TESTDATA>::object::test<Num>() {set_test_name(Name);
#define END } 
using namespace game;
namespace {
	const char * xml_file_name = "media\\shooter\\levels.xml";
}

namespace tut {
	struct ShooterData {
	};
	test_group<TESTDATA> shooterTests("Shooter tests");

	BEGIN(1, "Reading Static drome") 
		tinyxml2::XMLDocument doc;
		doc.LoadFile(xml_file_name);
		auto root = doc.RootElement();
		auto statics = root->FirstChildElement()->FirstChildElement("staticDromes");
		ensure_not_null(statics);
		auto first_static = statics->FirstChildElement();
		StaticDrome d(first_static);
		ensure_equals(d.left_top(),Vector2(200.0f,300.0f));
		ensure_equals(d.right_bottom(),Vector2(260,336));
	END

	BEGIN(2, "Reading drome List") 
		tinyxml2::XMLDocument doc;
		doc.LoadFile(xml_file_name);
		auto root = doc.RootElement();
		auto dromes = DromeList(root->FirstChildElement());
		ensure_not(dromes.statics().empty());
		const StaticDrome * first = dromes.statics().begin();
	END
}