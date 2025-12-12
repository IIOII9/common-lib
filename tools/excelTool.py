import tkinter as tk
from tkinter import filedialog, messagebox
import pandas as pd
import os

def select_file():
    file_path = filedialog.askopenfilename(
        filetypes=[("Excel Files", "*.xlsx;*.xls")],
        title="选择 Excel 文件"
    )
    if file_path:
        entry_file_path.delete(0, tk.END)
        entry_file_path.insert(0, file_path)

def process_excel():
    file_path = entry_file_path.get()
    x_value_str = entry_x.get()
    start_row_str = entry_start_row.get()

    if not file_path:
        messagebox.showerror("错误", "请先选择 Excel 文件")
        return

    if not os.path.isfile(file_path):
        messagebox.showerror("错误", "Excel 文件不存在")
        return

    try:
        X = float(x_value_str)
    except:
        messagebox.showerror("错误", "X 必须是数字")
        return

    try:
        start_row = int(start_row_str)
        if start_row < 1:
            raise ValueError
    except:
        messagebox.showerror("错误", "起始行必须为大于等于 1 的整数")
        return

    try:
        df = pd.read_excel(file_path, header=None)

        # 至少需要 A,B 列
        if df.shape[1] < 2:
            messagebox.showerror("错误", "Excel 至少需要两列 (A,B)")
            return

        # 自动补齐 C,D 列
        while df.shape[1] < 5:
            df[df.shape[1]] = ""

        # 计算起始行对应的 DataFrame 索引
        start_idx = start_row - 1

        # 分列
        base = df.iloc[start_idx:, 0]
        value = df.iloc[start_idx:, 1]

        maxValue = base + X
        minValue = base - X

        colC = []
        colD = []
        colE = []

        for b, v, maxv, minv in zip(base, value, maxValue, minValue):
            if v > maxv:
                outC = maxv
                outD = v - maxv
                outE = ""
            elif minv <= v <= maxv:
                outC = v
                outD = ""
                outE = ""
            else:
                outC = ""
                outD = ""
                outE = v


            colC.append(outC)
            colD.append(outD)
            colE.append(outE)

        # 写回处理后的数据
        df.iloc[start_idx:, 2] = colC
        df.iloc[start_idx:, 3] = colD
        df.iloc[start_idx:, 4] = colE

        # 自动另存文件
        dir_name, base_name = os.path.split(file_path)
        name, ext = os.path.splitext(base_name)
        output_path = os.path.join(dir_name, f"{name}_结果{ext}")

        df.to_excel(output_path, index=False)

        messagebox.showinfo("完成", f"处理完成\n已生成文件：\n{output_path}")

    except Exception as e:
        messagebox.showerror("错误", f"处理 Excel 时出错：\n{str(e)}")


# -------------------------
# Tkinter UI
# -------------------------
root = tk.Tk()
root.title("Excel 处理工具")
root.geometry("600x200")

# 文件路径
tk.Label(root, text="Excel 文件路径:").grid(row=0, column=0, padx=5, pady=5, sticky="e")
entry_file_path = tk.Entry(root, width=50)
entry_file_path.grid(row=0, column=1, padx=5, pady=5)

btn_select = tk.Button(root, text="选择文件", command=select_file)
btn_select.grid(row=0, column=2, padx=5, pady=5)


# X 输入
tk.Label(root, text="X 值:").grid(row=1, column=0, padx=5, pady=5, sticky="e")
entry_x = tk.Entry(root, width=20)
entry_x.grid(row=1, column=1, padx=5, pady=5, sticky="w")


# 起始行输入
tk.Label(root, text="起始行（从1开始）:").grid(row=2, column=0, padx=5, pady=5, sticky="e")
entry_start_row = tk.Entry(root, width=20)
entry_start_row.insert(0, "1")  # 默认从第1行开始
entry_start_row.grid(row=2, column=1, padx=5, pady=5, sticky="w")


# Run
btn_run = tk.Button(root, text="开始处理", command=process_excel, width=20)
btn_run.grid(row=3, column=1, padx=5, pady=10)

root.mainloop()
