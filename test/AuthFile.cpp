/**
* @file
* This file contains unit test suite related to the AuthFile module.
*
* @author xaqq
*/

#include <gtest/gtest.h>
#include <chrono>
#include <exception/moduleexception.hpp>
#include "tools/unixshellscript.hpp"
#include "core/auth/Interfaces/IAuthSourceMapper.hpp"
#include "modules/auth/auth-file/FileAuthSourceMapper.hpp"

/**
* Path to test-data file.
* this come from command line (see CMakeLists.txt)
*/
static std::string gl_data_path;

using namespace Leosac::Auth;
using namespace Leosac::Module::Auth;

namespace Leosac
{
    namespace Test
    {
        /**
        * Test the mapping of wiegand-card to user from a file.
        *
        * @note This test suite use the AuthFile-1.xml and AuthFile-2.xml files.
        */
        class AuthFileMapperTest : public ::testing::Test
        {
        public:
            AuthFileMapperTest() :
                    my_card_(new WiegandCard("aa:bb:cc:dd", 32)),
                    my_card2_(new WiegandCard("cc:dd:ee:ff", 32)),
                    unknown_card_(new WiegandCard("00:00:00:00", 32))
            {
                mapper_ = new FileAuthSourceMapper(gl_data_path + "AuthFile-1.xml");
            }

            ~AuthFileMapperTest()
            {
                delete mapper_;
            }

            IAuthSourceMapper *mapper_;
            IAuthenticationSourcePtr my_card_;
            IAuthenticationSourcePtr my_card2_;
            IAuthenticationSourcePtr unknown_card_;
        };

        /**
        * Successful mapping
        */
        TEST_F(AuthFileMapperTest, SimpleMapping)
        {
            ASSERT_FALSE(my_card_->owner().get());
            mapper_->mapToUser(my_card_);
            ASSERT_TRUE(my_card_->owner().get());
            ASSERT_EQ("MY_USER", my_card_->owner()->id());

            ASSERT_FALSE(my_card2_->owner().get());
            mapper_->mapToUser(my_card2_);
            ASSERT_TRUE(my_card2_->owner().get());
            ASSERT_EQ("Toto", my_card2_->owner()->id());
        }

        TEST_F(AuthFileMapperTest, TimeFrameMapping)
        {
            mapper_->mapToUser(my_card_);
            ASSERT_TRUE(my_card_->owner().get());
            IAccessProfilePtr profile = mapper_->buildProfile(my_card_);

            ASSERT_TRUE(profile.get());

            // Monday
            std::tm date = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            date.tm_hour = 12;
            date.tm_mday = 3;
            date.tm_mon = 10;
            date.tm_year = 114;
            std::time_t time_temp = std::mktime(&date);
            std::tm const *time_out = std::localtime(&time_temp);
            auto my_date = std::chrono::system_clock::from_time_t(time_temp);

            ASSERT_TRUE(profile->isAccessGranted(my_date, "doorA"));
            ASSERT_FALSE(profile->isAccessGranted(my_date, "doorB"));
        }

        /**
        * Card ID doesn't exist in the file.
        */
        TEST_F(AuthFileMapperTest, NotFoundMapping)
        {
            ASSERT_FALSE(unknown_card_->owner().get());
            mapper_->mapToUser(unknown_card_);
            ASSERT_FALSE(unknown_card_->owner().get());
        }

        /**
        * File is not accessible
        */
        TEST_F(AuthFileMapperTest, InvalidFile)
        {
            ASSERT_THROW(std::unique_ptr<IAuthSourceMapper> faulty_mapper(new FileAuthSourceMapper(gl_data_path + "no_file")),
                    ModuleException);
        }

        /**
        * AuthFile-2.xml has invalid content.
        */
        TEST_F(AuthFileMapperTest, InvalidFileContent)
        {
            ASSERT_THROW(
                    {
                        std::unique_ptr<IAuthSourceMapper> faulty_mapper(new FileAuthSourceMapper(gl_data_path + "AuthFile-2.xml"));
                        faulty_mapper->mapToUser(my_card_);
                    }, ModuleException);
        }
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    // gtest shall leave us with our arguments.
    // argv[1] shall be the path to test data file
    assert(argc == 2);
    gl_data_path = std::string(argv[1]) + '/';
    std::cout << "GLDATA = " << gl_data_path << std::endl;
    return RUN_ALL_TESTS();
}
