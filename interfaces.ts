export interface BoardStatus // 表示一块板子的状态
{
	ip: string;
	status: string;
	busy: boolean; // 是否空闲, false 表示空闲
}

export interface Result // 表示某些请求的结果
{
	success: boolean; // 表示是否成功
	message?: string; // 信息，可以没有
}

export interface BitStream
{
	bitstream: Buffer;
}

export interface BitStreamStructure
{
    design_name: string;
    part_name: string;
    date: string;
    time: string;
    raw_data_length: number;
    raw_data: Buffer; // 全部文件内容
}

export interface LEDStatusWSwitch
{
    leds?: number[];    // 56 个整数，分别描述 0~55 号 LED 状态，每位为 1 表示亮
                        // 共 8 个数码管，每数码管 7 个笔段，共 56 个灯与 LED 状态相同，可通过计算得到
    dip_switches?: number[];     // 32 个拨码开关，1 表示 ON，0~31 分别描述从左到右的开关状态
    micro_switches?: number[];   // 2 个微动开关，1 表示按下，0 描述 RESET 按钮状态，1 描述 CLOCK 按钮状态
    touch_switches?: number[];   // 4 个点触开关，1 表示按下，0~3 分别描述板子 E_K1~E_K4 的开关状态
}

