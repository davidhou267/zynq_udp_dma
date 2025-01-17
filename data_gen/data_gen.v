`timescale 1ns / 1ps
//
// Company: 
// Engineer: 
// 
// Create Date: 2024/02/27 16:35:14
// Design Name: 
// Module Name: data_gen
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description:   generate 2048 32bit data， transmit to PS DDR3 by AXI DMA
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//


module data_gen #(
    parameter TRANS_NUM = 32'd2047 //1514*1024
    )
    (
    input               clk           ,
    input               i_rstn          ,
    input               trans_start,

    input               M_AXIS_tready   ,
    output  [31 : 0]    M_AXIS_tdata     ,
    output              M_AXIS_tvalid   ,
    output              M_AXIS_tlast    ,
    output  [3  : 0]    M_AXIS_tkeep
    );

reg     [31 : 0]    r_M_AXIS_tdata   ;
reg                 r_M_AXIS_tvalid ;
reg                 r_M_AXIS_tlast  ;
reg     [3  : 0]    r_M_AXIS_tkeep  ;

reg     [1  : 0]    r_current_state ;
reg     [1  : 0]    r_next_state;

reg trans_start_0, trans_start_1;
wire pos_trans_start;
assign pos_trans_start = trans_start_0 & (~trans_start_1);
always @(posedge clk) begin
    if(!i_rstn) begin
        trans_start_0 <= 1'd0;
        trans_start_1 <= 1'd0;
    end
    else begin
        trans_start_0 <= trans_start;
        trans_start_1 <= trans_start_0;
    end
end


localparam IDLE = 2'd0;
localparam TRAN = 2'd1;
localparam LAST = 2'd2;

always @(posedge clk ) begin
    if(!i_rstn)
        r_current_state <= IDLE;
    else
        r_current_state <= r_next_state;
end

always @(*) begin
    case(r_current_state)
        IDLE : r_next_state = (pos_trans_start && M_AXIS_tready) ? TRAN : IDLE;
        TRAN : r_next_state = (r_M_AXIS_tdata == TRANS_NUM) ? LAST : TRAN;
        LAST : r_next_state = M_AXIS_tready ? IDLE : LAST;
        default : r_next_state = IDLE;
    endcase
end

always @(posedge clk ) begin
    case(r_current_state)
        IDLE : begin
            r_M_AXIS_tdata <= 32'd0;
            r_M_AXIS_tvalid <= 1'd0;
            r_M_AXIS_tlast <= 1'd0;
            r_M_AXIS_tkeep <= 4'b1111;
        end
        TRAN : begin
            r_M_AXIS_tvalid <= 1'd1;
            if(M_AXIS_tready)begin
                r_M_AXIS_tdata <= r_M_AXIS_tdata + 32'd1;
                if(r_M_AXIS_tdata == TRANS_NUM)
                    r_M_AXIS_tlast <= 1'd1;
                else
                    r_M_AXIS_tlast <= 1'd0;
            end
            else
                r_M_AXIS_tdata <= r_M_AXIS_tdata;
        end
        LAST : begin
            if(!M_AXIS_tready)begin
                r_M_AXIS_tvalid <= 1'd1;
                r_M_AXIS_tlast <= 1'd1;
                r_M_AXIS_tdata <= r_M_AXIS_tdata;
            end
            else begin
                r_M_AXIS_tvalid <= 1'd0;
                r_M_AXIS_tlast <= 1'd0;
                r_M_AXIS_tdata <= 32'd0;
            end
        end
        default : begin
            r_M_AXIS_tdata <= 32'd0;
            r_M_AXIS_tvalid <= 1'd0;
            r_M_AXIS_tlast <= 1'd0;
            r_M_AXIS_tkeep <= 4'b1111;
        end
    endcase
end

assign  M_AXIS_tdata     = r_M_AXIS_tdata     ;
assign  M_AXIS_tvalid   = r_M_AXIS_tvalid   ;
assign  M_AXIS_tlast    = r_M_AXIS_tlast    ;
assign  M_AXIS_tkeep    = r_M_AXIS_tkeep    ;

endmodule

