require_relative 'VSido'

VSido::connect();

angle = VSido::AngleRequest.new();
angle.cycle(10);
angle.angle(2,100);
ack =angle.execResult();
p ack.msg;

VSido::disConnect();

