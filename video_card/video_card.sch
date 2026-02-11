<?xml version="1.0" encoding="UTF-8"?>
<drawing version="7">
    <attr value="spartan3e" name="DeviceFamilyName">
        <trait delete="all:0" />
        <trait editname="all:0" />
        <trait edittrait="all:0" />
    </attr>
    <netlist>
        <signal name="clk" />
        <signal name="reset" />
        <signal name="XLXN_6(16:0)" />
        <signal name="hsync" />
        <signal name="vsync" />
        <signal name="rgb(2:0)" />
        <signal name="XLXN_11" />
        <signal name="XLXN_13(1:0)" />
        <signal name="XLXN_14" />
        <signal name="XLXN_15(16:0)" />
        <signal name="XLXN_16(1:0)" />
        <signal name="XLXN_21" />
        <signal name="XLXN_22" />
        <signal name="XLXN_23" />
        <signal name="XLXN_24" />
        <signal name="XLXN_25" />
        <signal name="sw1" />
        <signal name="sw0" />
        <signal name="start_async" />
        <signal name="leds(7:0)" />
        <port polarity="Input" name="clk" />
        <port polarity="Input" name="reset" />
        <port polarity="Output" name="hsync" />
        <port polarity="Output" name="vsync" />
        <port polarity="Output" name="rgb(2:0)" />
        <port polarity="Input" name="sw1" />
        <port polarity="Input" name="sw0" />
        <port polarity="Input" name="start_async" />
        <port polarity="Output" name="leds(7:0)" />
        <blockdef name="vga_sync">
            <timestamp>2026-2-6T17:22:31</timestamp>
            <rect width="256" x="64" y="-256" height="256" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="384" y1="-224" y2="-224" x1="320" />
            <line x2="384" y1="-160" y2="-160" x1="320" />
            <line x2="384" y1="-96" y2="-96" x1="320" />
            <rect width="64" x="320" y="-44" height="24" />
            <line x2="384" y1="-32" y2="-32" x1="320" />
        </blockdef>
        <blockdef name="rgb_decoder">
            <timestamp>2026-2-6T17:35:28</timestamp>
            <rect width="256" x="64" y="-256" height="256" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <rect width="64" x="0" y="-44" height="24" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <rect width="64" x="320" y="-236" height="24" />
            <line x2="384" y1="-224" y2="-224" x1="320" />
        </blockdef>
        <blockdef name="dual_port_ram">
            <timestamp>2026-2-6T17:49:46</timestamp>
            <rect width="256" x="64" y="-320" height="320" />
            <line x2="0" y1="-288" y2="-288" x1="64" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <rect width="64" x="0" y="-172" height="24" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <rect width="64" x="0" y="-108" height="24" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <rect width="64" x="0" y="-44" height="24" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <rect width="64" x="320" y="-300" height="24" />
            <line x2="384" y1="-288" y2="-288" x1="320" />
        </blockdef>
        <blockdef name="ram_populator">
            <timestamp>2026-2-6T18:36:39</timestamp>
            <rect width="304" x="64" y="-320" height="320" />
            <line x2="0" y1="-288" y2="-288" x1="64" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="432" y1="-288" y2="-288" x1="368" />
            <rect width="64" x="368" y="-172" height="24" />
            <line x2="432" y1="-160" y2="-160" x1="368" />
            <rect width="64" x="368" y="-44" height="24" />
            <line x2="432" y1="-32" y2="-32" x1="368" />
        </blockdef>
        <blockdef name="synchronizer_2_stage">
            <timestamp>2026-2-7T18:57:25</timestamp>
            <rect width="256" x="64" y="-128" height="128" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="384" y1="-96" y2="-96" x1="320" />
        </blockdef>
        <blockdef name="test_generator">
            <timestamp>2026-2-7T18:58:0</timestamp>
            <rect width="256" x="64" y="-64" height="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="384" y1="-32" y2="-32" x1="320" />
        </blockdef>
        <blockdef name="debug_led_display">
            <timestamp>2026-2-7T18:58:59</timestamp>
            <rect width="256" x="64" y="-128" height="128" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <rect width="64" x="320" y="-108" height="24" />
            <line x2="384" y1="-96" y2="-96" x1="320" />
        </blockdef>
        <block symbolname="vga_sync" name="XLXI_1">
            <blockpin signalname="clk" name="clk" />
            <blockpin signalname="reset" name="reset" />
            <blockpin signalname="hsync" name="hsync" />
            <blockpin signalname="vsync" name="vsync" />
            <blockpin signalname="XLXN_11" name="video_on" />
            <blockpin signalname="XLXN_6(16:0)" name="addr(16:0)" />
        </block>
        <block symbolname="rgb_decoder" name="XLXI_2">
            <blockpin signalname="clk" name="clk" />
            <blockpin signalname="reset" name="reset" />
            <blockpin signalname="XLXN_11" name="video_on" />
            <blockpin signalname="XLXN_13(1:0)" name="green_blue(1:0)" />
            <blockpin signalname="rgb(2:0)" name="rgb(2:0)" />
        </block>
        <block symbolname="dual_port_ram" name="XLXI_3">
            <blockpin signalname="clk" name="clk" />
            <blockpin signalname="XLXN_14" name="we" />
            <blockpin signalname="XLXN_15(16:0)" name="addr_w(16:0)" />
            <blockpin signalname="XLXN_16(1:0)" name="data_w(1:0)" />
            <blockpin signalname="XLXN_6(16:0)" name="addr_r(16:0)" />
            <blockpin signalname="XLXN_13(1:0)" name="data_r(1:0)" />
        </block>
        <block symbolname="ram_populator" name="XLXI_5">
            <blockpin signalname="clk" name="clk" />
            <blockpin signalname="XLXN_21" name="green" />
            <blockpin signalname="XLXN_22" name="blue" />
            <blockpin signalname="XLXN_23" name="data_ready" />
            <blockpin signalname="XLXN_24" name="start" />
            <blockpin signalname="XLXN_14" name="we" />
            <blockpin signalname="XLXN_16(1:0)" name="green_blue(1:0)" />
            <blockpin signalname="XLXN_15(16:0)" name="addr(16:0)" />
        </block>
        <block symbolname="synchronizer_2_stage" name="XLXI_6">
            <blockpin signalname="clk" name="clk" />
            <blockpin signalname="sw1" name="async" />
            <blockpin signalname="XLXN_21" name="sync" />
        </block>
        <block symbolname="synchronizer_2_stage" name="XLXI_7">
            <blockpin signalname="clk" name="clk" />
            <blockpin signalname="sw0" name="async" />
            <blockpin signalname="XLXN_22" name="sync" />
        </block>
        <block symbolname="synchronizer_2_stage" name="XLXI_8">
            <blockpin signalname="clk" name="clk" />
            <blockpin signalname="XLXN_25" name="async" />
            <blockpin signalname="XLXN_23" name="sync" />
        </block>
        <block symbolname="synchronizer_2_stage" name="XLXI_9">
            <blockpin signalname="clk" name="clk" />
            <blockpin signalname="start_async" name="async" />
            <blockpin signalname="XLXN_24" name="sync" />
        </block>
        <block symbolname="test_generator" name="XLXI_10">
            <blockpin signalname="clk" name="clk" />
            <blockpin signalname="XLXN_25" name="data_ready" />
        </block>
        <block symbolname="debug_led_display" name="XLXI_11">
            <blockpin signalname="clk" name="clk" />
            <blockpin signalname="XLXN_24" name="testpoint" />
            <blockpin signalname="leds(7:0)" name="leds(7:0)" />
        </block>
    </netlist>
    <sheet sheetnum="1" width="5440" height="3520">
        <instance x="4160" y="1664" name="XLXI_1" orien="R0">
        </instance>
        <instance x="3472" y="1728" name="XLXI_3" orien="R0">
        </instance>
        <branch name="clk">
            <wire x2="3472" y1="1440" y2="1440" x1="3440" />
        </branch>
        <iomarker fontsize="28" x="3440" y="1440" name="clk" orien="R180" />
        <branch name="clk">
            <wire x2="4160" y1="1440" y2="1440" x1="4128" />
        </branch>
        <iomarker fontsize="28" x="4128" y="1440" name="clk" orien="R180" />
        <branch name="reset">
            <wire x2="4160" y1="1632" y2="1632" x1="4128" />
        </branch>
        <iomarker fontsize="28" x="4128" y="1632" name="reset" orien="R180" />
        <branch name="clk">
            <wire x2="4160" y1="2016" y2="2016" x1="4112" />
        </branch>
        <branch name="XLXN_6(16:0)">
            <wire x2="3440" y1="1696" y2="1776" x1="3440" />
            <wire x2="4592" y1="1776" y2="1776" x1="3440" />
            <wire x2="3472" y1="1696" y2="1696" x1="3440" />
            <wire x2="4592" y1="1632" y2="1632" x1="4544" />
            <wire x2="4592" y1="1632" y2="1776" x1="4592" />
        </branch>
        <branch name="hsync">
            <wire x2="4576" y1="1440" y2="1440" x1="4544" />
        </branch>
        <iomarker fontsize="28" x="4576" y="1440" name="hsync" orien="R0" />
        <branch name="vsync">
            <wire x2="4576" y1="1504" y2="1504" x1="4544" />
        </branch>
        <iomarker fontsize="28" x="4576" y="1504" name="vsync" orien="R0" />
        <branch name="rgb(2:0)">
            <wire x2="4608" y1="2016" y2="2016" x1="4544" />
        </branch>
        <instance x="4160" y="2240" name="XLXI_2" orien="R0">
        </instance>
        <iomarker fontsize="28" x="4112" y="2016" name="clk" orien="R180" />
        <iomarker fontsize="28" x="4608" y="2016" name="rgb(2:0)" orien="R0" />
        <branch name="XLXN_11">
            <wire x2="4624" y1="1856" y2="1856" x1="3968" />
            <wire x2="3968" y1="1856" y2="2144" x1="3968" />
            <wire x2="4160" y1="2144" y2="2144" x1="3968" />
            <wire x2="4624" y1="1568" y2="1568" x1="4544" />
            <wire x2="4624" y1="1568" y2="1856" x1="4624" />
        </branch>
        <branch name="XLXN_13(1:0)">
            <wire x2="3888" y1="1440" y2="1440" x1="3856" />
            <wire x2="3888" y1="1440" y2="2208" x1="3888" />
            <wire x2="4032" y1="2208" y2="2208" x1="3888" />
            <wire x2="4160" y1="2208" y2="2208" x1="4032" />
        </branch>
        <instance x="2832" y="1792" name="XLXI_5" orien="R0">
        </instance>
        <branch name="XLXN_14">
            <wire x2="3472" y1="1504" y2="1504" x1="3264" />
        </branch>
        <branch name="XLXN_15(16:0)">
            <wire x2="3360" y1="1760" y2="1760" x1="3264" />
            <wire x2="3360" y1="1568" y2="1760" x1="3360" />
            <wire x2="3472" y1="1568" y2="1568" x1="3360" />
        </branch>
        <branch name="XLXN_16(1:0)">
            <wire x2="3472" y1="1632" y2="1632" x1="3264" />
        </branch>
        <branch name="clk">
            <wire x2="2832" y1="1504" y2="1504" x1="2800" />
        </branch>
        <iomarker fontsize="28" x="2800" y="1504" name="clk" orien="R180" />
        <instance x="2208" y="1872" name="XLXI_7" orien="R0">
        </instance>
        <instance x="2208" y="1664" name="XLXI_6" orien="R0">
        </instance>
        <instance x="2208" y="2096" name="XLXI_8" orien="R0">
        </instance>
        <instance x="2208" y="2320" name="XLXI_9" orien="R0">
        </instance>
        <branch name="clk">
            <wire x2="2208" y1="1568" y2="1568" x1="2176" />
        </branch>
        <iomarker fontsize="28" x="2176" y="1568" name="clk" orien="R180" />
        <branch name="clk">
            <wire x2="2208" y1="1776" y2="1776" x1="2176" />
        </branch>
        <iomarker fontsize="28" x="2176" y="1776" name="clk" orien="R180" />
        <branch name="clk">
            <wire x2="2208" y1="2000" y2="2000" x1="2176" />
        </branch>
        <iomarker fontsize="28" x="2176" y="2000" name="clk" orien="R180" />
        <branch name="clk">
            <wire x2="2208" y1="2224" y2="2224" x1="2176" />
        </branch>
        <iomarker fontsize="28" x="2176" y="2224" name="clk" orien="R180" />
        <branch name="XLXN_21">
            <wire x2="2832" y1="1568" y2="1568" x1="2592" />
        </branch>
        <branch name="XLXN_22">
            <wire x2="2704" y1="1776" y2="1776" x1="2592" />
            <wire x2="2704" y1="1632" y2="1776" x1="2704" />
            <wire x2="2832" y1="1632" y2="1632" x1="2704" />
        </branch>
        <branch name="XLXN_23">
            <wire x2="2736" y1="2000" y2="2000" x1="2592" />
            <wire x2="2736" y1="1696" y2="2000" x1="2736" />
            <wire x2="2832" y1="1696" y2="1696" x1="2736" />
        </branch>
        <branch name="XLXN_24">
            <wire x2="2784" y1="2224" y2="2224" x1="2592" />
            <wire x2="3008" y1="2224" y2="2224" x1="2784" />
            <wire x2="2832" y1="1760" y2="1760" x1="2784" />
            <wire x2="2784" y1="1760" y2="2224" x1="2784" />
        </branch>
        <branch name="XLXN_25">
            <wire x2="2208" y1="2064" y2="2064" x1="2080" />
        </branch>
        <branch name="clk">
            <wire x2="1680" y1="2064" y2="2064" x1="1648" />
            <wire x2="1696" y1="2064" y2="2064" x1="1680" />
        </branch>
        <branch name="sw1">
            <wire x2="2208" y1="1632" y2="1632" x1="2176" />
        </branch>
        <iomarker fontsize="28" x="2176" y="1632" name="sw1" orien="R180" />
        <branch name="sw0">
            <wire x2="2208" y1="1840" y2="1840" x1="2176" />
        </branch>
        <iomarker fontsize="28" x="2176" y="1840" name="sw0" orien="R180" />
        <branch name="start_async">
            <wire x2="2208" y1="2288" y2="2288" x1="2176" />
        </branch>
        <iomarker fontsize="28" x="2176" y="2288" name="start_async" orien="R180" />
        <branch name="clk">
            <wire x2="3008" y1="2160" y2="2160" x1="2992" />
        </branch>
        <branch name="leds(7:0)">
            <wire x2="3456" y1="2160" y2="2160" x1="3392" />
        </branch>
        <iomarker fontsize="28" x="3456" y="2160" name="leds(7:0)" orien="R0" />
        <instance x="3008" y="2256" name="XLXI_11" orien="R0">
        </instance>
        <iomarker fontsize="28" x="2992" y="2160" name="clk" orien="R180" />
        <instance x="1696" y="2096" name="XLXI_10" orien="R0">
        </instance>
        <iomarker fontsize="28" x="1648" y="2064" name="clk" orien="R180" />
        <branch name="reset">
            <wire x2="4160" y1="2080" y2="2080" x1="4128" />
        </branch>
        <iomarker fontsize="28" x="4128" y="2080" name="reset" orien="R180" />
    </sheet>
</drawing>