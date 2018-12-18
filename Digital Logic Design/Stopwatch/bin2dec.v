`timescale 1ns / 1ps

module bin2dec(
    input [7:0] binvalue,
    output reg [13:0] decvalue
    );
    
    always @ (*) begin
        decvalue = binvalue[7:4];
        decvalue = decvalue * 10;
        decvalue = decvalue + binvalue[3:0];
        decvalue = decvalue * 100;
    end
    
endmodule