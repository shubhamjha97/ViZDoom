#include <boost/python.hpp>
#include "ViziaDoomGamePython.h"
#include "ViziaDoomDefines.h"
#include "ViziaDoomExceptions.h"
#include "ViziaDoomUtilities.h"
#include <exception>
#include <vector>

using namespace Vizia;

namespace bp = boost::python;

/* C++ code to expose DoomGamePython via python */

PyObject* createExceptionClass(const char* name, PyObject* baseTypeObj = PyExc_Exception) {
    using std::string;

	std::string scopeName = bp::extract<std::string>(bp::scope().attr("__name__"));
	std::string qualifiedName0 = scopeName + "." + name;
    char* qualifiedName1 = const_cast<char*>(qualifiedName0.c_str());

    PyObject* typeObj = PyErr_NewException(qualifiedName1, baseTypeObj, 0);
    if(!typeObj) bp::throw_error_already_set();
    bp::scope().attr(name) = bp::handle<>(bp::borrowed(typeObj));
    return typeObj;
}

#define EXCEPTION_TRANSLATE_TO_PYT(n) PyObject* type ## n = NULL; \
void translate ## n (Vizia::Exception const &e){ PyErr_SetString( type ## n  , e.what()); }
/*
 * PyObject* typeMyException = NULL;
 * void translate(Vizia::Exception const &e) { PyErr_SetString(typeMyException, e.what()); }
 */

EXCEPTION_TRANSLATE_TO_PYT(DoomUnexpectedExitException)
EXCEPTION_TRANSLATE_TO_PYT(DoomIsNotRunningException)
EXCEPTION_TRANSLATE_TO_PYT(DoomErrorException)
EXCEPTION_TRANSLATE_TO_PYT(SharedMemoryException)
EXCEPTION_TRANSLATE_TO_PYT(MessageQueueException)
EXCEPTION_TRANSLATE_TO_PYT(PathDoesNotExistsException)


/* DoomGamePython methods overloading */

void (DoomGamePython::*addAvailableButton1)(Button) = &DoomGamePython::addAvailableButton;
void (DoomGamePython::*addAvailableButton2)(Button, int) = &DoomGamePython::addAvailableButton;

void (DoomGamePython::*advanceAction1)() = &DoomGamePython::advanceAction;
void (DoomGamePython::*advanceAction2)(unsigned int) = &DoomGamePython::advanceAction;
void (DoomGamePython::*advanceAction3)(unsigned int, bool, bool) = &DoomGamePython::advanceAction;

double (DoomGamePython::*makeAction1)(boost::python::list const &) = &DoomGamePython::makeAction;
double (DoomGamePython::*makeAction2)(boost::python::list const &, unsigned int) = &DoomGamePython::makeAction;

BOOST_PYTHON_MODULE(vizia)
{
    using namespace boost::python;
    Py_Initialize();
    bp::numeric::array::set_module_and_type("numpy", "ndarray");
    import_array();
    
    /* exceptions */

#define EXCEPTION_TO_PYT(n, pytn) type ## n = createExceptionClass(#pytn); \
bp::register_exception_translator<Vizia:: n >(&translate ## n );
	/* typeMyException = createExceptionClass("myException");
	 * bp::register_exception_translator<Vizia::myException>(&translate);
	 */

	EXCEPTION_TO_PYT(DoomUnexpectedExitException, doom_unexpected_exit_exception)
	EXCEPTION_TO_PYT(DoomIsNotRunningException, doom_is_not_running_exception)
	EXCEPTION_TO_PYT(DoomErrorException, doom_error_exception)
	EXCEPTION_TO_PYT(SharedMemoryException, shared_memory_exception)
	EXCEPTION_TO_PYT(MessageQueueException, message_queue_exception)
	EXCEPTION_TO_PYT(PathDoesNotExistsException, path_does_not_exists_exception)

#define ENUM_VAL_2_PYT(v) .value( #v , v )
    /* .value("VALUE_IN_PYTHON", VALUE_IN_CPP) */

    enum_<Mode>("Mode")
        ENUM_VAL_2_PYT(PLAYER)
        ENUM_VAL_2_PYT(SPECTATOR)
		ENUM_VAL_2_PYT(ASYNC_PLAYER)
		ENUM_VAL_2_PYT(ASYNC_SPECTATOR);

	enum_<ScreenFormat>("ScreenFormat")
        ENUM_VAL_2_PYT(CRCGCB)
        ENUM_VAL_2_PYT(CRCGCBDB)
        ENUM_VAL_2_PYT(RGB24)
        ENUM_VAL_2_PYT(RGBA32)
        ENUM_VAL_2_PYT(ARGB32)
        ENUM_VAL_2_PYT(CBCGCR)
        ENUM_VAL_2_PYT(CBCGCRDB)
        ENUM_VAL_2_PYT(BGR24)
        ENUM_VAL_2_PYT(BGRA32)
        ENUM_VAL_2_PYT(ABGR32)
        ENUM_VAL_2_PYT(GRAY8)
        ENUM_VAL_2_PYT(DEPTH_BUFFER8)
        ENUM_VAL_2_PYT(DOOM_256_COLORS8);
    
    enum_<ScreenResolution>("ScreenResolution")
        ENUM_VAL_2_PYT(RES_40X30)
        ENUM_VAL_2_PYT(RES_60X45)
        ENUM_VAL_2_PYT(RES_80X50)
        ENUM_VAL_2_PYT(RES_80X60)
        ENUM_VAL_2_PYT(RES_100X75)
        ENUM_VAL_2_PYT(RES_120X75)
        ENUM_VAL_2_PYT(RES_120X90)
        ENUM_VAL_2_PYT(RES_160X100)
        ENUM_VAL_2_PYT(RES_160X120)
        ENUM_VAL_2_PYT(RES_200X120)
        ENUM_VAL_2_PYT(RES_200X150)
        ENUM_VAL_2_PYT(RES_240X135)
        ENUM_VAL_2_PYT(RES_240X150)
        ENUM_VAL_2_PYT(RES_240X180)
        ENUM_VAL_2_PYT(RES_256X144)
        ENUM_VAL_2_PYT(RES_256X160)
        ENUM_VAL_2_PYT(RES_256X192)
        ENUM_VAL_2_PYT(RES_320X200)
        ENUM_VAL_2_PYT(RES_320X240)
        ENUM_VAL_2_PYT(RES_400X225)    // 16:9
        ENUM_VAL_2_PYT(RES_400X300)
        ENUM_VAL_2_PYT(RES_480X270)    // 16:9
        ENUM_VAL_2_PYT(RES_480X360)
        ENUM_VAL_2_PYT(RES_512X288)    // 16:9
        ENUM_VAL_2_PYT(RES_512X384)
        ENUM_VAL_2_PYT(RES_640X360)    // 16:9
        ENUM_VAL_2_PYT(RES_640X400)
        ENUM_VAL_2_PYT(RES_640X480)
        ENUM_VAL_2_PYT(RES_720X480)    // 16:10
        ENUM_VAL_2_PYT(RES_720X540)
        ENUM_VAL_2_PYT(RES_800X450)    // 16:9
        ENUM_VAL_2_PYT(RES_800X480)
        ENUM_VAL_2_PYT(RES_800X500)    // 16:10
        ENUM_VAL_2_PYT(RES_800X600)
        ENUM_VAL_2_PYT(RES_848X480)    // 16:9
        ENUM_VAL_2_PYT(RES_960X600)    // 16:10
        ENUM_VAL_2_PYT(RES_960X720)
        ENUM_VAL_2_PYT(RES_1024X576)    // 16:9
        ENUM_VAL_2_PYT(RES_1024X600)    // 17:10
        ENUM_VAL_2_PYT(RES_1024X640)    // 16:10
        ENUM_VAL_2_PYT(RES_1024X768)
        ENUM_VAL_2_PYT(RES_1088X612)    // 16:9
        ENUM_VAL_2_PYT(RES_1152X648)    // 16:9
        ENUM_VAL_2_PYT(RES_1152X720)    // 16:10
        ENUM_VAL_2_PYT(RES_1152X864)
        ENUM_VAL_2_PYT(RES_1280X720)    // 16:9
        ENUM_VAL_2_PYT(RES_1280X854)
        ENUM_VAL_2_PYT(RES_1280X800)    // 16:10
        ENUM_VAL_2_PYT(RES_1280X960)
        ENUM_VAL_2_PYT(RES_1280X1024)    // 5:4
        ENUM_VAL_2_PYT(RES_1360X768)    // 16:9
        ENUM_VAL_2_PYT(RES_1366X768)
        ENUM_VAL_2_PYT(RES_1400X787)    // 16:9
        ENUM_VAL_2_PYT(RES_1400X875)    // 16:10
        ENUM_VAL_2_PYT(RES_1400X1050)
        ENUM_VAL_2_PYT(RES_1440X900)
        ENUM_VAL_2_PYT(RES_1440X960)
        ENUM_VAL_2_PYT(RES_1440X1080)
        ENUM_VAL_2_PYT(RES_1600X900)    // 16:9
        ENUM_VAL_2_PYT(RES_1600X1000)    // 16:10
        ENUM_VAL_2_PYT(RES_1600X1200)
        ENUM_VAL_2_PYT(RES_1680X1050)    // 16:10
        ENUM_VAL_2_PYT(RES_1920X1080)
        ENUM_VAL_2_PYT(RES_1920X1200)
        ENUM_VAL_2_PYT(RES_2048X1536)
        ENUM_VAL_2_PYT(RES_2560X1440)
        ENUM_VAL_2_PYT(RES_2560X1600)
        ENUM_VAL_2_PYT(RES_2560X2048)
        ENUM_VAL_2_PYT(RES_2880X1800)
        ENUM_VAL_2_PYT(RES_3200X1800)
        ENUM_VAL_2_PYT(RES_3840X2160)
        ENUM_VAL_2_PYT(RES_3840X2400)
        ENUM_VAL_2_PYT(RES_4096X2160)
        ENUM_VAL_2_PYT(RES_5120X2880);

	enum_<Button>("Button")
        ENUM_VAL_2_PYT(ATTACK)
        ENUM_VAL_2_PYT(USE)
        ENUM_VAL_2_PYT(JUMP)
        ENUM_VAL_2_PYT(CROUCH)
        ENUM_VAL_2_PYT(TURN180)
        ENUM_VAL_2_PYT(ALTATTACK)
        ENUM_VAL_2_PYT(RELOAD)
        ENUM_VAL_2_PYT(ZOOM)
        ENUM_VAL_2_PYT(SPEED)
        ENUM_VAL_2_PYT(STRAFE)
        ENUM_VAL_2_PYT(MOVE_RIGHT)
        ENUM_VAL_2_PYT(MOVE_LEFT)
        ENUM_VAL_2_PYT(MOVE_BACKWARD)
        ENUM_VAL_2_PYT(MOVE_FORWARD)
        ENUM_VAL_2_PYT(TURN_RIGHT)
        ENUM_VAL_2_PYT(TURN_LEFT)
        ENUM_VAL_2_PYT(LOOK_UP)
        ENUM_VAL_2_PYT(LOOK_DOWN)
        ENUM_VAL_2_PYT(LAND)
        ENUM_VAL_2_PYT(SELECT_WEAPON1)
        ENUM_VAL_2_PYT(SELECT_WEAPON2)
        ENUM_VAL_2_PYT(SELECT_WEAPON3)
        ENUM_VAL_2_PYT(SELECT_WEAPON4)
        ENUM_VAL_2_PYT(SELECT_WEAPON5)
        ENUM_VAL_2_PYT(SELECT_WEAPON6)
        ENUM_VAL_2_PYT(SELECT_WEAPON7)
        ENUM_VAL_2_PYT(SELECT_WEAPON8)
        ENUM_VAL_2_PYT(SELECT_WEAPON9)
        ENUM_VAL_2_PYT(SELECT_WEAPON0)
        ENUM_VAL_2_PYT(SELECT_NEXT_WEAPON)
        ENUM_VAL_2_PYT(SELECT_PREV_WEAPON)
        ENUM_VAL_2_PYT(DROP_SELECTED_WEAPON)
        ENUM_VAL_2_PYT(ACTIVATE_SELECTED_ITEM)
        ENUM_VAL_2_PYT(SELECT_NEXT_ITEM)
        ENUM_VAL_2_PYT(SELECT_PREV_ITEM)
        ENUM_VAL_2_PYT(DROP_SELECTED_ITEM)
        ENUM_VAL_2_PYT(LOOK_UP_DOWN_DELTA)
        ENUM_VAL_2_PYT(TURN_LEFT_RIGHT_DELTA)
        ENUM_VAL_2_PYT(MOVE_FORWARD_BACKWARD_DELTA)
        ENUM_VAL_2_PYT(MOVE_LEFT_RIGHT_DELTA)
        ENUM_VAL_2_PYT(MOVE_UP_DOWN_DELTA);        
        
    enum_<GameVariable>("GameVariable")
        ENUM_VAL_2_PYT(KILLCOUNT)
        ENUM_VAL_2_PYT(ITEMCOUNT)
        ENUM_VAL_2_PYT(SECRETCOUNT)
        ENUM_VAL_2_PYT(FRAGCOUNT)
        ENUM_VAL_2_PYT(HEALTH)
        ENUM_VAL_2_PYT(ARMOR)
        ENUM_VAL_2_PYT(DEAD)
        ENUM_VAL_2_PYT(ON_GROUND)
        ENUM_VAL_2_PYT(ATTACK_READY)
        ENUM_VAL_2_PYT(ALTATTACK_READY)
        ENUM_VAL_2_PYT(SELECTED_WEAPON)
        ENUM_VAL_2_PYT(SELECTED_WEAPON_AMMO)
        ENUM_VAL_2_PYT(AMMO1)
        ENUM_VAL_2_PYT(AMMO2)
        ENUM_VAL_2_PYT(AMMO3)
        ENUM_VAL_2_PYT(AMMO4)
        ENUM_VAL_2_PYT(AMMO5)
        ENUM_VAL_2_PYT(AMMO6)
        ENUM_VAL_2_PYT(AMMO7)
        ENUM_VAL_2_PYT(AMMO8)
        ENUM_VAL_2_PYT(AMMO9)
        ENUM_VAL_2_PYT(AMMO0)
        ENUM_VAL_2_PYT(WEAPON1)
        ENUM_VAL_2_PYT(WEAPON2)
        ENUM_VAL_2_PYT(WEAPON3)
        ENUM_VAL_2_PYT(WEAPON4)
        ENUM_VAL_2_PYT(WEAPON5)
        ENUM_VAL_2_PYT(WEAPON6)
        ENUM_VAL_2_PYT(WEAPON7)
        ENUM_VAL_2_PYT(WEAPON8)
        ENUM_VAL_2_PYT(WEAPON9)
        ENUM_VAL_2_PYT(WEAPON0)
        ENUM_VAL_2_PYT(USER1)
        ENUM_VAL_2_PYT(USER2)
        ENUM_VAL_2_PYT(USER3)
        ENUM_VAL_2_PYT(USER4)
        ENUM_VAL_2_PYT(USER5)
        ENUM_VAL_2_PYT(USER6)
        ENUM_VAL_2_PYT(USER7)
        ENUM_VAL_2_PYT(USER8)
        ENUM_VAL_2_PYT(USER9)
        ENUM_VAL_2_PYT(USER10)
        ENUM_VAL_2_PYT(USER11)
        ENUM_VAL_2_PYT(USER12)
        ENUM_VAL_2_PYT(USER13)
        ENUM_VAL_2_PYT(USER14)
        ENUM_VAL_2_PYT(USER15)
        ENUM_VAL_2_PYT(USER16)
        ENUM_VAL_2_PYT(USER17)
        ENUM_VAL_2_PYT(USER18)
        ENUM_VAL_2_PYT(USER19)
        ENUM_VAL_2_PYT(USER20)
        ENUM_VAL_2_PYT(USER21)
        ENUM_VAL_2_PYT(USER22)
        ENUM_VAL_2_PYT(USER23)
        ENUM_VAL_2_PYT(USER24)
        ENUM_VAL_2_PYT(USER25)
        ENUM_VAL_2_PYT(USER26)
        ENUM_VAL_2_PYT(USER27)
        ENUM_VAL_2_PYT(USER28)
        ENUM_VAL_2_PYT(USER29)
        ENUM_VAL_2_PYT(USER30);

	def("doom_tics_to_ms", DoomTicsToMs);
	def("ms_to_doom_tics", MsToDoomTics);
    def("doom_fixed_to_double", DoomFixedToDouble);
    
    class_<GameStatePython>("GameState", no_init)
        .def_readonly("number", &GameStatePython::number)
        .def_readonly("image_buffer", &GameStatePython::imageBuffer)
        .def_readonly("game_variables", &GameStatePython::gameVariables);

    class_<DoomGamePython>("DoomGame", init<>())
		.def("init", &DoomGamePython::init)
		.def("load_config", &DoomGamePython::loadConfig)
		.def("close", &DoomGamePython::close)
		.def("new_episode", &DoomGamePython::newEpisode)
		.def("is_episode_finished", &DoomGamePython::isEpisodeFinished)
		.def("is_new_episode", &DoomGamePython::isNewEpisode)
		.def("is_player_dead", &DoomGamePython::isPlayerDead)
		.def("respawn_player", &DoomGamePython::respawnPlayer)
		.def("set_action", &DoomGamePython::setAction)
        .def("make_action", makeAction1)
        .def("make_action", makeAction2)
		.def("advance_action", advanceAction1)
        .def("advance_action", advanceAction2)
		.def("advance_action", advanceAction3)
        
		.def("get_state", &DoomGamePython::getState)
    
        .def("get_game_variable", &DoomGamePython::getGameVariable)
        .def("get_game_screen", &DoomGamePython::getGameScreen)

        .def("get_living_reward", &DoomGamePython::getLivingReward)
        .def("set_living_reward", &DoomGamePython::setLivingReward)
        
        .def("get_death_penalty", &DoomGamePython::getDeathPenalty)
        .def("set_death_penalty", &DoomGamePython::setDeathPenalty)
        
        .def("get_last_reward", &DoomGamePython::getLastReward)
        .def("get_summary_reward", &DoomGamePython::getSummaryReward)
        
        .def("get_last_action", &DoomGamePython::getLastAction)
        
		.def("add_available_game_variable", &DoomGamePython::addAvailableGameVariable)
		.def("clear_available_game_variables", &DoomGamePython::clearAvailableGameVariables)
        .def("get_available_game_variables_size", &DoomGamePython::getAvailableGameVariablesSize)

		.def("add_available_button", addAvailableButton1)
        .def("add_available_button", addAvailableButton2)

		.def("clear_available_buttons", &DoomGamePython::clearAvailableButtons)
        .def("get_available_buttons_size", &DoomGamePython::getAvailableButtonsSize)
        .def("set_button_max_value", &DoomGamePython::setButtonMaxValue)
		.def("get_button_max_value", &DoomGamePython::getButtonMaxValue)

        .def("add_game_args", &DoomGamePython::addGameArgs)
        .def("clear_game_args", &DoomGamePython::clearGameArgs)

        .def("send_game_command", &DoomGamePython::sendGameCommand)

        .def("get_mode", &DoomGamePython::getMode)
        .def("set_mode", &DoomGamePython::setMode)

		.def("set_doom_engine_path", &DoomGamePython::setDoomEnginePath)
		.def("set_doom_game_path", &DoomGamePython::setDoomGamePath)
		.def("set_doom_scenario_path", &DoomGamePython::setDoomScenarioPath)
		.def("set_doom_map", &DoomGamePython::setDoomMap)
		.def("set_doom_skill", &DoomGamePython::setDoomSkill)
		.def("set_doom_config_path", &DoomGamePython::setDoomConfigPath)

        .def("get_seed", &DoomGamePython::getSeed)
        .def("set_seed", &DoomGamePython::setSeed)

        .def("get_episode_start_time", &DoomGamePython::getEpisodeStartTime)
        .def("set_episode_start_time", &DoomGamePython::setEpisodeStartTime)
		.def("get_episode_timeout", &DoomGamePython::getEpisodeTimeout)
		.def("set_episode_timeout", &DoomGamePython::setEpisodeTimeout)
		.def("get_episode_time", &DoomGamePython::getEpisodeTime)
		
        .def("set_console_enabled",&DoomGamePython::setConsoleEnabled)
        
		.def("set_screen_resolution", &DoomGamePython::setScreenResolution)
		.def("set_screen_format", &DoomGamePython::setScreenFormat)
		.def("set_render_hud", &DoomGamePython::setRenderHud)
		.def("set_render_weapon", &DoomGamePython::setRenderWeapon)
		.def("set_render_crosshair", &DoomGamePython::setRenderCrosshair)
		.def("set_render_decals", &DoomGamePython::setRenderDecals)
		.def("set_render_particles", &DoomGamePython::setRenderParticles)
		.def("get_screen_width", &DoomGamePython::getScreenWidth)
		.def("get_screen_height", &DoomGamePython::getScreenHeight)
        .def("get_screen_channels", &DoomGamePython::getScreenChannels)
		.def("get_screen_size", &DoomGamePython::getScreenSize)
		.def("get_screen_pitch", &DoomGamePython::getScreenPitch)
		.def("get_screen_format", &DoomGamePython::getScreenFormat)
        .def("set_window_visible", &DoomGamePython::setWindowVisible);
}
