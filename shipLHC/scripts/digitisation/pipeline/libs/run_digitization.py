import subprocess
import logging
import time
import os
import resource

def run_digitization(directories, run_number, mode):
    tag = f"[run {run_number:06d}]"

    input_root_file = (directories['converted'] / f"run{run_number:06d}" / f"run{run_number:06d}_converted.root")

    source_advsndsw = "source /opt/run4/software/setUp.sh"
    alienv = f"cd {directories['advsndsw']} && eval $(alienv load advsndsw/latest --no-refresh)"
    # Different tb have different mapping
    mapping_file = (
        "shipLHC/digitisation/rawToDigi/mapping/detector_info_tb_5_2026.csv"
        if run_number <= 386
        else "shipLHC/digitisation/rawToDigi/mapping/detector_info_tb_7_2026.csv"
    )


    output_root_file = (directories['converted'] / f"run{run_number:06d}" / f"run{run_number:06d}_digi_{mode}.root")

    command = f"""
    {source_advsndsw} &&
    {alienv} &&
    executable="$ADVSNDSW_ROOT/bin/run_raw_to_digi" &&
    detinfo_csv="$ADVSNDSW_ROOT/{mapping_file}" &&
    "$executable" "{input_root_file}" "$detinfo_csv" "{output_root_file}" "{mode}"
    """

    logging.debug("%s Running Digitization (%s): %s", tag, mode, command)

    start = time.perf_counter()

    result = subprocess.run(
        command,
        shell=True,
        executable="/bin/bash",
        capture_output=True,
        text=True
    )

    duration = time.perf_counter() - start

    if result.stdout:
        logging.info("%s [%s] stdout:\n%s", tag, mode, result.stdout)

    if result.stderr:
        logging.error("%s [%s] stderr:\n%s", tag, mode, result.stderr)

    logging.info("%s [%s] finished in %.2f seconds", tag, mode, duration)

    # Get file size in MB
    try:
        file_size_mb = os.path.getsize(output_root_file) / (1024 ** 2)
        logging.info("%s [%s] output file size: %.2f MB", tag, mode, file_size_mb)
    except FileNotFoundError:
        logging.warning("%s [%s] output file not found", tag, mode)
        file_size_mb = None

    # Get resource usage of current process
    usage = resource.getrusage(resource.RUSAGE_CHILDREN)
    max_memory_mb = usage.ru_maxrss / 1024  # Convert KB to MB
    logging.info("%s [%s] Max memory: %.2f MB", tag, mode, max_memory_mb)

    return duration, file_size_mb, max_memory_mb