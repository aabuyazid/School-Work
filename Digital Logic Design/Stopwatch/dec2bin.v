`timescale 1ns / 1ps

module dec2bin(
    input [13:0] decvalue,
    output reg [15:0] binvalue 
    );
    
    reg [13:0] temp;
    
    always @ (*) begin
        temp = decvalue; 
        binvalue[3:0] = temp % 10;
        temp = temp/10;
        binvalue[7:4] = temp % 10;
        temp = temp/10;
        binvalue[11:8] = temp % 10;
        temp = temp/10;
        binvalue[15:12] = temp % 10;
    end
        
endmodule
