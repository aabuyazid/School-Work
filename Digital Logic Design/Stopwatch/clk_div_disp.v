`timescale 1ns / 1ps

module clk_div_disp(
    input clk,
    output slow_clk
    );
    
    reg [16:0] COUNT; // Arbitrary number, GO CHANGE IT
    
    assign slow_clk = COUNT[16];
    
    always @(posedge clk) begin
        COUNT = COUNT + 1;
    end
    
endmodule