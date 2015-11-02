/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.5
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


public class AngleRequest extends Request {
  private long swigCPtr;

  protected AngleRequest(long cPtr, boolean cMemoryOwn) {
    super(VSidoConnectJNI.AngleRequest_SWIGUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  protected static long getCPtr(AngleRequest obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        VSidoConnectJNI.delete_AngleRequest(swigCPtr);
      }
      swigCPtr = 0;
    }
    super.delete();
  }

  public AngleRequest() {
    this(VSidoConnectJNI.new_AngleRequest(), true);
  }

  public void cycle(int cyc) {
    VSidoConnectJNI.AngleRequest_cycle(swigCPtr, this, cyc);
  }

  public void angle(short arg0, int deg) {
    VSidoConnectJNI.AngleRequest_angle(swigCPtr, this, arg0, deg);
  }

}
