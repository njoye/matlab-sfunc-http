/*  File    : sfunc.cpp (4.09.2017)
 *  Abstract:
 *          This file serves as demo on how to use cURL in MatLab System-Functions.
 *          More Info on GitHub: https://github.com/njoye/matlab-sfunc-http
 */

/**
 * 
 *  This project requires prerequisits. Specifically:
 *      1. curl/curl.h -> "cURL", used for getting the websites content -> cURL needs to be installed on your system from the begininng
 *      
 *      Also: This function will get executed 5 times. Everytime, a new cURL Request will be made and a new random number will be visible on the display 
 */

#include "mex.h" //mex functions like mexPrintf
#include <iostream> //Standard iostream, don't know if this is necessary, could be though
#include <string> //String for using ... well, strings
#include <curl/curl.h> //Include cURL for easier HTTP Requests


#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  sfunc 

/*
 * Need to include simstruc.h for the definition of the SimStruct and
 * its associated macro definitions.
 */
#include "simstruc.h"

#define IS_PARAM_DOUBLE(pVal) (mxIsNumeric(pVal) && !mxIsLogical(pVal) &&\
!mxIsEmpty(pVal) && !mxIsSparse(pVal) && !mxIsComplex(pVal) && mxIsDouble(pVal))


/*====================*
 * S-function methods *
 *====================*/

#define MDL_CHECK_PARAMETERS
#if defined(MDL_CHECK_PARAMETERS)  && defined(MATLAB_MEX_FILE)
/*
 * Check to make sure that each parameter is 1-d and positive
 */
static void mdlCheckParameters(SimStruct *S)
{
    //No need to check parameters because there are .. none :)
}
#endif


/* Function: mdlInitializeSizes ===============================================
 * Abstract:
 *    The sizes information is used by Simulink to determine the S-function
 *    block's characteristics (number of inputs, outputs, states, etc.).
 *
 */
static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 0);  /* Number of expected parameters */
#if defined(MATLAB_MEX_FILE)
    if (ssGetNumSFcnParams(S) == ssGetSFcnParamsCount(S)) {
        mdlCheckParameters(S); //Will not do anything, because I removed the code in there
        if (ssGetErrorStatus(S) != NULL) {
            return;
        }
    } else {
        return; /* Parameter mismatch will be reported by Simulink */
    }
#endif
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0); //Didn't understand what this is...

    if (!ssSetNumInputPorts(S, 0)) return; //Sets the amount of input ports => 0
    
    if (!ssSetNumOutputPorts(S, 1)) return; //Sets the amount of output Ports
    
    /* Setting the output ports width's */
    ssSetOutputPortWidth(S,0,1); //Setting the output port (idx=0) width to 1

    ssSetNumSampleTimes(S, 1); 
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 0); 
    ssSetNumModes(S, 0); 
    ssSetNumNonsampledZCs(S, 0); 

    ssSetSimStateCompliance(S, SIM_STATE_COMPLIANCE_UNKNOWN); 

    ssSetOptions(S, 0); 
}



/* Function: mdlInitializeSampleTimes =========================================
 * Abstract:
 *    This function is used to specify the sample time(s) for your
 *    S-function. You must register the same number of sample times as
 *    specified in ssSetNumSampleTimes.
 */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, 1); 
    ssSetOffsetTime(S, 0, 0.0);
    ssSetModelReferenceSampleTimeDefaultInheritance(S); 
}

#define MDL_START  /* Change to #undef to remove function */
#if defined(MDL_START) 
  /* Function: mdlStart =======================================================
   * Abstract:
   *    This function is called once at start of model execution. If you
   *    have states that should be initialized once, this is the place
   *    to do it.
   */
  static void mdlStart(SimStruct *S)
  {
      //Start code here (like constructing your object 'n stuff)
  }
#endif /*  MDL_START */
  

//WriteCallback, used for returning the output of the website/api
size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp){
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
  
/* Function: mdlOutputs =======================================================
 * Abstract:
 *    In this function, you compute the outputs of your S-function
 *    block.
 */
static void mdlOutputs(SimStruct *S, int_T tid)
{
    real_T  *port0 = ssGetOutputPortRealSignal(S,0); //Port0 as OutPut
    
    CURLcode res; //Initialize the result "cURL Code" Object
    
    curl_global_init(CURL_GLOBAL_ALL); //globally initating cURL
    
    CURL* easyhandle = curl_easy_init(); //Initiating the cURL req
    std::string readBuffer; //initiating the readBuffer where the output will be saved later on
    
    curl_easy_setopt(easyhandle, CURLOPT_URL, "https://www.random.org/integers/?num=1&min=1&max=100&col=5&base=10&format=plain&rnd=new"); //setting the url (returns a random number in plain html)
    
    curl_easy_setopt(easyhandle, CURLOPT_SSL_VERIFYPEER, 0L); //Decide whether you want to check for proper SSL Encryption or not, could increase the complexity of your code, but will also make it more secure
    curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, WriteCallback); //defining the WriteCallback that gets called when the data returns from the website
    
    curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &readBuffer); //set the variable into which the data will be saved later on

    res = curl_easy_perform(easyhandle); //performing the cURL req
    //"res" now contains the returned cURL Code (CURLE_OK or if CURLOPT_ERRORBUFFER was set: readable error message in the buffer)
    
    //curl_easy_cleanup(easyhandle); //Cleaning up afterwards - didn't do this in my code because my model would only run once, not a bad idea though

    port0[0] = std::stoi(readBuffer); //Converting/Casting and setting the output value to the given number from the website
}                                                




/* I emtpied or skipped the below functions because I didn't need them. sfuntmpl.doc from Matlab contains more info on these and other functions. */

#ifdef MATLAB_MEX_FILE
/* For now mdlG[S]etSimState are only supported in normal simulation */

/* Define to indicate that this S-Function has the mdlG[S]etSimState mothods */
#define MDL_SIM_STATE

/* Function: mdlGetSimState =====================================================
 * Abstract:
 *
 */

static mxArray* mdlGetSimState(SimStruct* S)
{
    return 0;
}
 
 

/* Function: mdlGetSimState =====================================================
 * Abstract:
 *
 */
static void mdlSetSimState(SimStruct* S, const mxArray* ma)
{
   
}

#endif


/* Function: mdlTerminate =====================================================
 * Abstract:
 *    In this function, you should perform any actions that are necessary
 *    at the termination of a simulation.  For example, if memory was
 *    allocated in mdlStart, this is the place to free it.
 */
static void mdlTerminate(SimStruct *S)
{

}
/*======================================================*
 * See sfuntmpl.doc for the optional S-function methods *
 *======================================================*/

/*=============================*
 * Required S-function trailer *
 *=============================*/




#ifdef  MATLAB_MEX_FILE    
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif

