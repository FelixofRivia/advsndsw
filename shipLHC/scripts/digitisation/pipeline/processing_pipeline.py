from pathlib import Path
import logging
import sys
import csv
import time

from libs.run_digitization import run_digitization
from libs.run_dqm import run_dqm

def main():
    directories = {
        "raw" : Path("/eos/experiment/sndlhc/Run4/testbeam2026/raw_data"),
        "converted" : Path("/eos/experiment/sndlhc/Run4/testbeam2026/digi_benchmark/converted_data"),
        "histos" : Path("/eos/experiment/sndlhc/Run4/testbeam2026/digi_benchmark"),
        "geometry" : Path("/eos/experiment/sndlhc/Run4/testbeam2026/converted_data/geofile_testbeam_2026.root"),
        "logs" : Path("/eos/experiment/sndlhc/www/testbeam2026/logs"),
        "cmssw_src" : Path("/home/filippo/CMSSW_15_1_1/src"),
        "advsndsw" : Path("/home/filippo")
    }

    logging.basicConfig(
        level=getattr(logging, "DEBUG"),
        format="%(asctime)s [%(levelname)s] %(message)s",
        datefmt="%H:%M:%S",
        handlers=[
            logging.FileHandler(directories["logs"] / "processing_pipeline.log", mode="a"),  # append mode
            logging.StreamHandler(sys.stdout)          # still print to terminal
        ]
    )

    logging.info("Directories configuration:\n%s", "\n".join(f"[{k}] -> {v}" for k, v in directories.items()))

    runs = [int(p.name.replace("run", "")) for p in directories["raw"].glob("run*") if int(p.name.replace("run", "")) >= 273]

    # CSV file path
    csv_file = directories["logs"] / "benchmarks.csv"
    
    # CSV headers
    headers = [
        "run_number",
        "digitization_ttree_time_s",
        "digitization_ttree_size_mb",
        "digitization_ttree_memory_mb",
        "digitization_rntuple_time_s",
        "digitization_rntuple_size_mb",
        "digitization_rntuple_memory_mb",
        "dqm_ttree_1thread_time_s",
        "dqm_ttree_1thread_memory_mb",
        "dqm_ttree_2thread_time_s",
        "dqm_ttree_2thread_memory_mb",
        "dqm_ttree_3thread_time_s",
        "dqm_ttree_3thread_memory_mb",
        "dqm_ttree_4thread_time_s",
        "dqm_ttree_4thread_memory_mb",
        "dqm_rntuple_1thread_time_s",
        "dqm_rntuple_1thread_memory_mb",
        "dqm_rntuple_2thread_time_s",
        "dqm_rntuple_2thread_memory_mb",
        "dqm_rntuple_3thread_time_s",
        "dqm_rntuple_3thread_memory_mb",
        "dqm_rntuple_4thread_time_s",
        "dqm_rntuple_4thread_memory_mb",
    ]

    try:
        # Open CSV file in append mode
        with open(csv_file, mode='a', newline='') as f:
            writer = csv.DictWriter(f, fieldnames=headers)
            
            # Write header only if file is empty
            if f.tell() == 0:
                writer.writeheader()
            
            for run_number in runs:
                logging.info("Selected run: %s", run_number)
                
                rtg_t_ttree, rtg_size_ttree, rtg_mem_ttree = run_digitization(directories, run_number, "ttree")
                rtg_t_rntuple, rtg_size_rntuple, rtg_mem_rntuple = run_digitization(directories, run_number, "rntuple")
                dqm_t_ttree_1, dqm_mem_ttree_1 = run_dqm(directories, run_number, "ttree", 1)
                dqm_t_ttree_2, dqm_mem_ttree_2 = run_dqm(directories, run_number, "ttree", 2)
                dqm_t_ttree_3, dqm_mem_ttree_3 = run_dqm(directories, run_number, "ttree", 3)
                dqm_t_ttree_4, dqm_mem_ttree_4 = run_dqm(directories, run_number, "ttree", 4)
                dqm_t_rntuple_1, dqm_mem_rntuple_1 = run_dqm(directories, run_number, "rntuple", 1)
                dqm_t_rntuple_2, dqm_mem_rntuple_2 = run_dqm(directories, run_number, "rntuple", 2)
                dqm_t_rntuple_3, dqm_mem_rntuple_3 = run_dqm(directories, run_number, "rntuple", 3)
                dqm_t_rntuple_4, dqm_mem_rntuple_4 = run_dqm(directories, run_number, "rntuple", 4)

                
                # Write row to CSV
                row = {
                    "run_number": run_number,
                    "digitization_ttree_time_s": f"{rtg_t_ttree:.2f}" if rtg_t_ttree is not None else "",
                    "digitization_ttree_size_mb": f"{rtg_size_ttree:.2f}" if rtg_size_ttree is not None else "",
                    "digitization_ttree_memory_mb": f"{rtg_mem_ttree:.2f}" if rtg_mem_ttree is not None else "",
                    "digitization_rntuple_time_s": f"{rtg_t_rntuple:.2f}" if rtg_t_rntuple is not None else "",
                    "digitization_rntuple_size_mb": f"{rtg_size_rntuple:.2f}" if rtg_size_rntuple is not None else "",
                    "digitization_rntuple_memory_mb": f"{rtg_mem_rntuple:.2f}" if rtg_mem_rntuple is not None else "",
                    "dqm_ttree_1thread_time_s": f"{dqm_t_ttree_1:.2f}" if dqm_t_ttree_1 is not None else "",
                    "dqm_ttree_1thread_memory_mb": f"{dqm_mem_ttree_1:.2f}" if dqm_mem_ttree_1 is not None else "",
                    "dqm_ttree_2thread_time_s": f"{dqm_t_ttree_2:.2f}" if dqm_t_ttree_2 is not None else "",
                    "dqm_ttree_2thread_memory_mb": f"{dqm_mem_ttree_2:.2f}" if dqm_mem_ttree_2 is not None else "",
                    "dqm_ttree_3thread_time_s": f"{dqm_t_ttree_3:.2f}" if dqm_t_ttree_3 is not None else "",
                    "dqm_ttree_3thread_memory_mb": f"{dqm_mem_ttree_3:.2f}" if dqm_mem_ttree_3 is not None else "",
                    "dqm_ttree_4thread_time_s": f"{dqm_t_ttree_4:.2f}" if dqm_t_ttree_4 is not None else "",
                    "dqm_ttree_4thread_memory_mb": f"{dqm_mem_ttree_4:.2f}" if dqm_mem_ttree_4 is not None else "",
                    "dqm_rntuple_1thread_time_s": f"{dqm_t_rntuple_1:.2f}" if dqm_t_rntuple_1 is not None else "",
                    "dqm_rntuple_1thread_memory_mb": f"{dqm_mem_rntuple_1:.2f}" if dqm_mem_rntuple_1 is not None else "",
                    "dqm_rntuple_2thread_time_s": f"{dqm_t_rntuple_2:.2f}" if dqm_t_rntuple_2 is not None else "",
                    "dqm_rntuple_2thread_memory_mb": f"{dqm_mem_rntuple_2:.2f}" if dqm_mem_rntuple_2 is not None else "",
                    "dqm_rntuple_3thread_time_s": f"{dqm_t_rntuple_3:.2f}" if dqm_t_rntuple_3 is not None else "",
                    "dqm_rntuple_3thread_memory_mb": f"{dqm_mem_rntuple_3:.2f}" if dqm_mem_rntuple_3 is not None else "",
                    "dqm_rntuple_4thread_time_s": f"{dqm_t_rntuple_4:.2f}" if dqm_t_rntuple_4 is not None else "",
                    "dqm_rntuple_4thread_memory_mb": f"{dqm_mem_rntuple_4:.2f}" if dqm_mem_rntuple_4 is not None else "",
                }
                writer.writerow(row)
                f.flush()
                logging.info("Benchmarks for run %s written to CSV", run_number)

    except KeyboardInterrupt:
        logging.info("Pipeline loop stopped by user")

    except Exception:
        logging.exception("Error while processing pipeline loop")

if __name__ == "__main__":
    main()