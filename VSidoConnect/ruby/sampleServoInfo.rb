require_relative 'VSido'

VSido::connect();

servoReq = VSido::ServoInfoRequest.new();
servoReq.info(1,0,54);
servoReq.info(2,0,54);
servoInfo =servoReq.execResult();
p servoInfo.info;

VSido::disConnect();

