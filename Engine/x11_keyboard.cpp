#include "keyboard.hpp"

#if __linux__

#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

using namespace std;

#define Button6 6
#define Button7 7

int keycodes[256], eventTime[256], scancodes[GLFW_KEY_LAST + 1];
bool state[GLFW_KEY_LAST + 1];

static int translateKeyCode(int scancode){
  Display* display = glfwGetX11Display();
  int keySym;

  // Valid key code range is  [8,255], according to the Xlib manual
  if (scancode < 8 || scancode > 255)
    return GLFW_KEY_UNKNOWN;

  // Try secondary keysym, for numeric keypad keys
  // Note: This way we always force "NumLock = ON", which is intentional
  // since the returned key code should correspond to a physical
  // location.
  keySym = XkbKeycodeToKeysym(display, scancode, 0, 1);
  switch (keySym)
  {
    case XK_KP_0:           return GLFW_KEY_KP_0;
    case XK_KP_1:           return GLFW_KEY_KP_1;
    case XK_KP_2:           return GLFW_KEY_KP_2;
    case XK_KP_3:           return GLFW_KEY_KP_3;
    case XK_KP_4:           return GLFW_KEY_KP_4;
    case XK_KP_5:           return GLFW_KEY_KP_5;
    case XK_KP_6:           return GLFW_KEY_KP_6;
    case XK_KP_7:           return GLFW_KEY_KP_7;
    case XK_KP_8:           return GLFW_KEY_KP_8;
    case XK_KP_9:           return GLFW_KEY_KP_9;
    case XK_KP_Separator:
    case XK_KP_Decimal:     return GLFW_KEY_KP_DECIMAL;
    case XK_KP_Equal:       return GLFW_KEY_KP_EQUAL;
    case XK_KP_Enter:       return GLFW_KEY_KP_ENTER;
    default:                break;
  }

  // Now try primary keysym for function keys (non-printable keys)
  // These should not depend on the current keyboard layout
  keySym = XkbKeycodeToKeysym(display, scancode, 0, 0);
  

  switch (keySym)
  {
    case XK_Escape:         return GLFW_KEY_ESCAPE;
    case XK_Tab:            return GLFW_KEY_TAB;
    case XK_Shift_L:        return GLFW_KEY_LEFT_SHIFT;
    case XK_Shift_R:        return GLFW_KEY_RIGHT_SHIFT;
    case XK_Control_L:      return GLFW_KEY_LEFT_CONTROL;
    case XK_Control_R:      return GLFW_KEY_RIGHT_CONTROL;
    case XK_Meta_L:
    case XK_Alt_L:          return GLFW_KEY_LEFT_ALT;
    case XK_Mode_switch: // Mapped to Alt_R on many keyboards
    case XK_ISO_Level3_Shift: // AltGr on at least some machines
    case XK_Meta_R:
    case XK_Alt_R:          return GLFW_KEY_RIGHT_ALT;
    case XK_Super_L:        return GLFW_KEY_LEFT_SUPER;
    case XK_Super_R:        return GLFW_KEY_RIGHT_SUPER;
    case XK_Menu:           return GLFW_KEY_MENU;
    case XK_Num_Lock:       return GLFW_KEY_NUM_LOCK;
    case XK_Caps_Lock:      return GLFW_KEY_CAPS_LOCK;
    case XK_Print:          return GLFW_KEY_PRINT_SCREEN;
    case XK_Scroll_Lock:    return GLFW_KEY_SCROLL_LOCK;
    case XK_Pause:          return GLFW_KEY_PAUSE;
    case XK_Delete:         return GLFW_KEY_DELETE;
    case XK_BackSpace:      return GLFW_KEY_BACKSPACE;
    case XK_Return:         return GLFW_KEY_ENTER;
    case XK_Home:           return GLFW_KEY_HOME;
    case XK_End:            return GLFW_KEY_END;
    case XK_Page_Up:        return GLFW_KEY_PAGE_UP;
    case XK_Page_Down:      return GLFW_KEY_PAGE_DOWN;
    case XK_Insert:         return GLFW_KEY_INSERT;
    case XK_Left:           return GLFW_KEY_LEFT;
    case XK_Right:          return GLFW_KEY_RIGHT;
    case XK_Down:           return GLFW_KEY_DOWN;
    case XK_Up:             return GLFW_KEY_UP;
    case XK_F1:             return GLFW_KEY_F1;
    case XK_F2:             return GLFW_KEY_F2;
    case XK_F3:             return GLFW_KEY_F3;
    case XK_F4:             return GLFW_KEY_F4;
    case XK_F5:             return GLFW_KEY_F5;
    case XK_F6:             return GLFW_KEY_F6;
    case XK_F7:             return GLFW_KEY_F7;
    case XK_F8:             return GLFW_KEY_F8;
    case XK_F9:             return GLFW_KEY_F9;
    case XK_F10:            return GLFW_KEY_F10;
    case XK_F11:            return GLFW_KEY_F11;
    case XK_F12:            return GLFW_KEY_F12;
    case XK_F13:            return GLFW_KEY_F13;
    case XK_F14:            return GLFW_KEY_F14;
    case XK_F15:            return GLFW_KEY_F15;
    case XK_F16:            return GLFW_KEY_F16;
    case XK_F17:            return GLFW_KEY_F17;
    case XK_F18:            return GLFW_KEY_F18;
    case XK_F19:            return GLFW_KEY_F19;
    case XK_F20:            return GLFW_KEY_F20;
    case XK_F21:            return GLFW_KEY_F21;
    case XK_F22:            return GLFW_KEY_F22;
    case XK_F23:            return GLFW_KEY_F23;
    case XK_F24:            return GLFW_KEY_F24;
    case XK_F25:            return GLFW_KEY_F25;

    // Numeric keypad
    case XK_KP_Divide:      return GLFW_KEY_KP_DIVIDE;
    case XK_KP_Multiply:    return GLFW_KEY_KP_MULTIPLY;
    case XK_KP_Subtract:    return GLFW_KEY_KP_SUBTRACT;
    case XK_KP_Add:         return GLFW_KEY_KP_ADD;

    // These should have been detected in secondary keysym test above!
    case XK_KP_Insert:      return GLFW_KEY_KP_0;
    case XK_KP_End:         return GLFW_KEY_KP_1;
    case XK_KP_Down:        return GLFW_KEY_KP_2;
    case XK_KP_Page_Down:   return GLFW_KEY_KP_3;
    case XK_KP_Left:        return GLFW_KEY_KP_4;
    case XK_KP_Right:       return GLFW_KEY_KP_6;
    case XK_KP_Home:        return GLFW_KEY_KP_7;
    case XK_KP_Up:          return GLFW_KEY_KP_8;
    case XK_KP_Page_Up:     return GLFW_KEY_KP_9;
    case XK_KP_Delete:      return GLFW_KEY_KP_DECIMAL;
    case XK_KP_Equal:       return GLFW_KEY_KP_EQUAL;
    case XK_KP_Enter:       return GLFW_KEY_KP_ENTER;

    // Last resort: Check for printable keys (should not happen if the XKB
    // extension is available). This will give a layout dependent mapping
    // (which is wrong, and we may miss some keys, especially on non-US
    // keyboards), but it's better than nothing...
    case XK_a:              return GLFW_KEY_A;
    case XK_b:              return GLFW_KEY_B;
    case XK_c:              return GLFW_KEY_C;
    case XK_d:              return GLFW_KEY_D;
    case XK_e:              return GLFW_KEY_E;
    case XK_f:              return GLFW_KEY_F;
    case XK_g:              return GLFW_KEY_G;
    case XK_h:              return GLFW_KEY_H;
    case XK_i:              return GLFW_KEY_I;
    case XK_j:              return GLFW_KEY_J;
    case XK_k:              return GLFW_KEY_K;
    case XK_l:              return GLFW_KEY_L;
    case XK_m:              return GLFW_KEY_M;
    case XK_n:              return GLFW_KEY_N;
    case XK_o:              return GLFW_KEY_O;
    case XK_p:              return GLFW_KEY_P;
    case XK_q:              return GLFW_KEY_Q;
    case XK_r:              return GLFW_KEY_R;
    case XK_s:              return GLFW_KEY_S;
    case XK_t:              return GLFW_KEY_T;
    case XK_u:              return GLFW_KEY_U;
    case XK_v:              return GLFW_KEY_V;
    case XK_w:              return GLFW_KEY_W;
    case XK_x:              return GLFW_KEY_X;
    case XK_y:              return GLFW_KEY_Y;
    case XK_z:              return GLFW_KEY_Z;
    case XK_1:              return GLFW_KEY_1;
    case XK_2:              return GLFW_KEY_2;
    case XK_3:              return GLFW_KEY_3;
    case XK_4:              return GLFW_KEY_4;
    case XK_5:              return GLFW_KEY_5;
    case XK_6:              return GLFW_KEY_6;
    case XK_7:              return GLFW_KEY_7;
    case XK_8:              return GLFW_KEY_8;
    case XK_9:              return GLFW_KEY_9;
    case XK_0:              return GLFW_KEY_0;
    case XK_space:          return GLFW_KEY_SPACE;
    case XK_minus:          return GLFW_KEY_MINUS;
    case XK_equal:          return GLFW_KEY_EQUAL;
    case XK_bracketleft:    return GLFW_KEY_LEFT_BRACKET;
    case XK_bracketright:   return GLFW_KEY_RIGHT_BRACKET;
    case XK_backslash:      return GLFW_KEY_BACKSLASH;
    case XK_semicolon:      return GLFW_KEY_SEMICOLON;
    case XK_apostrophe:     return GLFW_KEY_APOSTROPHE;
    case XK_grave:          return GLFW_KEY_GRAVE_ACCENT;
    case XK_comma:          return GLFW_KEY_COMMA;
    case XK_period:         return GLFW_KEY_PERIOD;
    case XK_slash:          return GLFW_KEY_SLASH;
    case XK_less:           return GLFW_KEY_WORLD_1; // At least in some layouts...
    default:                break;
  }

  // No matching translation was found
  return GLFW_KEY_UNKNOWN;
}

void initKeyboard(GLFWwindow* w){
  int scancode, key;
  Display* display = glfwGetX11Display();

  memset(keycodes, -1, sizeof(keycodes));
  memset(scancodes, -1, sizeof(scancodes));
  memset(state, 0, sizeof(state));
  memset(eventTime, 0, sizeof(eventTime));

  if (true) {
    // Use XKB to determine physical key locations independently of the current
    // keyboard layout

    char name[XkbKeyNameLength + 1];
    XkbDescPtr desc = XkbGetMap(display, 0, XkbUseCoreKbd);
    XkbGetNames(display, XkbKeyNamesMask, desc);

    // Find the X11 key code -> GLFW key code mapping
    for (scancode = desc->min_key_code;  scancode <= desc->max_key_code;  scancode++){
      memcpy(name, desc->names->keys[scancode].name, XkbKeyNameLength);
      name[XkbKeyNameLength] = '\0';

      // Map the key name to a GLFW key code. Note: We only map printable
      // keys here, and we use the US keyboard layout. The rest of the
      // keys (function keys) are mapped using traditional KeySym
      // translations.
      if (strcmp(name, "TLDE") == 0) key = GLFW_KEY_GRAVE_ACCENT;
      else if (strcmp(name, "AE01") == 0) key = GLFW_KEY_1;
      else if (strcmp(name, "AE02") == 0) key = GLFW_KEY_2;
      else if (strcmp(name, "AE03") == 0) key = GLFW_KEY_3;
      else if (strcmp(name, "AE04") == 0) key = GLFW_KEY_4;
      else if (strcmp(name, "AE05") == 0) key = GLFW_KEY_5;
      else if (strcmp(name, "AE06") == 0) key = GLFW_KEY_6;
      else if (strcmp(name, "AE07") == 0) key = GLFW_KEY_7;
      else if (strcmp(name, "AE08") == 0) key = GLFW_KEY_8;
      else if (strcmp(name, "AE09") == 0) key = GLFW_KEY_9;
      else if (strcmp(name, "AE10") == 0) key = GLFW_KEY_0;
      else if (strcmp(name, "AE11") == 0) key = GLFW_KEY_MINUS;
      else if (strcmp(name, "AE12") == 0) key = GLFW_KEY_EQUAL;
      else if (strcmp(name, "AD01") == 0) key = GLFW_KEY_Q;
      else if (strcmp(name, "AD02") == 0) key = GLFW_KEY_W;
      else if (strcmp(name, "AD03") == 0) key = GLFW_KEY_E;
      else if (strcmp(name, "AD04") == 0) key = GLFW_KEY_R;
      else if (strcmp(name, "AD05") == 0) key = GLFW_KEY_T;
      else if (strcmp(name, "AD06") == 0) key = GLFW_KEY_Y;
      else if (strcmp(name, "AD07") == 0) key = GLFW_KEY_U;
      else if (strcmp(name, "AD08") == 0) key = GLFW_KEY_I;
      else if (strcmp(name, "AD09") == 0) key = GLFW_KEY_O;
      else if (strcmp(name, "AD10") == 0) key = GLFW_KEY_P;
      else if (strcmp(name, "AD11") == 0) key = GLFW_KEY_LEFT_BRACKET;
      else if (strcmp(name, "AD12") == 0) key = GLFW_KEY_RIGHT_BRACKET;
      else if (strcmp(name, "AC01") == 0) key = GLFW_KEY_A;
      else if (strcmp(name, "AC02") == 0) key = GLFW_KEY_S;
      else if (strcmp(name, "AC03") == 0) key = GLFW_KEY_D;
      else if (strcmp(name, "AC04") == 0) key = GLFW_KEY_F;
      else if (strcmp(name, "AC05") == 0) key = GLFW_KEY_G;
      else if (strcmp(name, "AC06") == 0) key = GLFW_KEY_H;
      else if (strcmp(name, "AC07") == 0) key = GLFW_KEY_J;
      else if (strcmp(name, "AC08") == 0) key = GLFW_KEY_K;
      else if (strcmp(name, "AC09") == 0) key = GLFW_KEY_L;
      else if (strcmp(name, "AC10") == 0) key = GLFW_KEY_SEMICOLON;
      else if (strcmp(name, "AC11") == 0) key = GLFW_KEY_APOSTROPHE;
      else if (strcmp(name, "AB01") == 0) key = GLFW_KEY_Z;
      else if (strcmp(name, "AB02") == 0) key = GLFW_KEY_X;
      else if (strcmp(name, "AB03") == 0) key = GLFW_KEY_C;
      else if (strcmp(name, "AB04") == 0) key = GLFW_KEY_V;
      else if (strcmp(name, "AB05") == 0) key = GLFW_KEY_B;
      else if (strcmp(name, "AB06") == 0) key = GLFW_KEY_N;
      else if (strcmp(name, "AB07") == 0) key = GLFW_KEY_M;
      else if (strcmp(name, "AB08") == 0) key = GLFW_KEY_COMMA;
      else if (strcmp(name, "AB09") == 0) key = GLFW_KEY_PERIOD;
      else if (strcmp(name, "AB10") == 0) key = GLFW_KEY_SLASH;
      else if (strcmp(name, "BKSL") == 0) key = GLFW_KEY_BACKSLASH;
      else if (strcmp(name, "LSGT") == 0) key = GLFW_KEY_WORLD_1;
      else key = GLFW_KEY_UNKNOWN;

      if ((scancode >= 0) && (scancode < 256))
        keycodes[scancode] = key;
    }

    XkbFreeNames(desc, XkbKeyNamesMask, True);
    XkbFreeKeyboard(desc, 0, True);
  }

  for (scancode = 0;  scancode < 256;  scancode++){
    // Translate the un-translated key codes using traditional X11 KeySym
    // lookups
    if (keycodes[scancode] < 0)
      keycodes[scancode] = translateKeyCode(scancode);

    // Store the reverse translation for faster key name lookup
    if (keycodes[scancode] > 0)
      scancodes[keycodes[scancode]] = scancode;
  }
  unsigned long event_mask = FocusChangeMask | KeyPressMask | KeyReleaseMask | ButtonPressMask  | ButtonReleaseMask | PointerMotionMask;
  Window window = glfwGetX11Window(w);
  //XSelectInput(display,window,event_mask);

  //cout <<  CirculateNotify << " " << ConfigureNotify << " " << CreateNotify << " " << DestroyNotify << " " << GravityNotify << " " << MapNotify << " " << ReparentNotify << " " << UnmapNotify << " " << " " << Expose << " " << FocusIn << " " << FocusOut << " " << NoExpose << " " << GraphicsExpose << " " << PropertyNotify << " " << ResizeRequest << " " << ClientMessage << " " << SelectionRequest << " " << SelectionNotify << " " << endl; 
}

int getKey(GLFWwindow* w, int scancode){
  return state[scancode];
}
void setKey(int key, bool b){
  state[key] = b;
}

string event2str(int type){
  if(type == MotionNotify)
    return "MotionNotify";
  else if(type == ButtonPress)
    return "ButtonPress";
  else if(type == ButtonRelease)
    return "ButtonRelease";
  else if(type == KeymapNotify)
    return "KeymapNotify";
  else if(type == KeyPress)
    return "KeyPress";
  else if(type == KeyRelease)
    return "KeyRelease";
  else if(type == VisibilityNotify)
    return "VisibilityNotify";
  else if(type == ButtonPress)
    return "ButtonPress";
  else if(type == ButtonPress)
    return "ButtonPress";
  else
    return "Unknown";
}

bool motionCallbackActive = false;
GLFWcursorposfun motionCallback = NULL;
GLFWwindow* motionWindow = NULL;

bool mouseCallbackActive = false;
GLFWmousebuttonfun mouseCallback = NULL;
GLFWwindow* mouseWindow = NULL;

int translateState(int state){
    int mods = 0;

    if (state & ShiftMask)
        mods |= GLFW_MOD_SHIFT;
    if (state & ControlMask)
        mods |= GLFW_MOD_CONTROL;
    if (state & Mod1Mask)
        mods |= GLFW_MOD_ALT;
    if (state & Mod4Mask)
        mods |= GLFW_MOD_SUPER;

    return mods;
}

int translateButton(int b){
  switch(b){
    case Button1: return GLFW_MOUSE_BUTTON_LEFT;
    case Button2: return GLFW_MOUSE_BUTTON_MIDDLE;
    case Button3: return GLFW_MOUSE_BUTTON_RIGHT;
    case Button4:
    case Button5:
    case Button6:
    case Button7:
      return -1;
    default:
      return b - Button1 - 4;

  }
}

void pollEvents(){
  Display* display = glfwGetX11Display();
  int count = XPending(display);
  vector<XEvent> events;
  while (count--){
    XEvent event;
    XNextEvent(display, &event);
    if (event.type == KeyPress || event.type == KeyRelease){
      int keycode = event.xkey.keycode;
      //cout << keycode << " - " << keycodes[keycode] << ((event.type == KeyPress) ? " KeyPress " : " KeyRelease ") << event.xkey.time <<  endl;
      if(event.type != KeyPress || (signed) event.xkey.time > eventTime[keycode]){
        eventTime[keycode] = event.xkey.time;
        int scancode = keycodes[keycode];
        if(event.type == KeyPress)
          state[scancode] = true;
        if(event.type == KeyRelease){
          state[scancode] = false;
        }
      }
    }else if(event.type == MotionNotify){
      if(motionCallbackActive)
        motionCallback(motionWindow, event.xkey.x, event.xkey.y);

    }else if(event.type == ButtonPress || event.type == ButtonRelease){

      int action = ((event.type == ButtonPress) ? GLFW_PRESS : GLFW_RELEASE);
      int button = translateButton(event.xbutton.button);
      int mods = translateState(event.xbutton.state);

      if(button != -1 && mouseCallbackActive)
        mouseCallback(mouseWindow, button, action, mods);
      else if(button == -1){
        //TODO: Scroll
      }
    }else{
      if(event.type != MotionNotify)
        events.push_back(event);
    }
    for(int i=0; i<(signed)events.size(); i++)
      XPutBackEvent(display, &events[i]);

  }
  XFlush(display);
  glfwPollEvents();
}

void setCursorPosCallback(GLFWwindow* window, GLFWcursorposfun cursorMoveCallback){
  motionCallback = cursorMoveCallback;
  motionCallbackActive = true;
  motionWindow = window;
}

void setMouseButtonCallback(GLFWwindow* window, GLFWmousebuttonfun mouseButtonCallback){
  mouseCallback = mouseButtonCallback;
  mouseCallbackActive = true;
  mouseWindow = window;
}

#else

void pollEvents(){
  glfwPollEvents();
}
void initKeyboard(){

}

int getKey(GLFWwindow* w, int scancode){
  return glfwGetKey(w, scancode);
}
void setKey(int scancode, bool b){
}
void setCursorPosCallback(GLFWwindow* window, GLFWcursorposfun cursorMoveCallback){
  glfwSetCursorPosCallback(window, cursorMoveCallback);
}
void setMouseButtonCallback(GLFWwindow* window, GLFWmousebuttonfun mouseButtonCallback){
  glfwSetMouseButtonCallback(window, mouseButtonCallback);
}

#endif
